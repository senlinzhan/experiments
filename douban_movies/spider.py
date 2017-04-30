#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# author: senlinzhan@gmail.com
#
import os.path
import logging
import requests
import random
import re
import os
import shutil
from bs4 import BeautifulSoup
from app import db
from app.models import Movie, Comment
from requests.packages.urllib3.exceptions import InsecureRequestWarning
from collections import deque


class PageNotFoundError(Exception):
    '''
    页面未找到
    '''

    pass


class RequestPageError(Exception):
    '''
    页面请求出错
    '''

    pass


base_dir_path = os.path.abspath(os.path.dirname(__file__))


class SimpleSpider:
    '''
    简易的爬虫模板
    '''

    # 模拟浏览器的请求
    headers = [
        {
            'User-Agent': 'Mozilla/5.0 (X11; Ubuntu; Linux i686; rv:44.0) Gecko/20100101 Firefox/44.0',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Connection': 'keep-alive'
        }, 
        {
            'User-Agent': 'Mozilla/5.0 (Macintosh; Intel Mac OS X 10_9_3) AppleWebKit/537.75.14 (KHTML, like Gecko) Version/7.0.3 Safari/7046A194A',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Connection': 'keep-alive'
        },
        {
            'User-Agent': 'Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0)',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Connection': 'keep-alive'
        },
        {
            'User-Agent': 'Mozilla/5.0 (Windows NT 6.3; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/41.0.2226.0 Safari/537.36',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Connection': 'keep-alive'
        },
        {
            'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/42.0.2311.135 Safari/537.36 Edge/12.246',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Connection': 'keep-alive'
        },
        {
            'User-Agent': 'Opera/9.80 (Windows NT 6.0) Presto/2.12.388 Version/12.14',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Connection': 'keep-alive'
        },
        {
            'User-Agent': ' Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.0; Trident/4.0; TencentTraveler 4.0; Trident/4.0; SLCC1; Media Center PC 5.0; .NET CLR 2.0.50727; .NET CLR 3.5.30729; .NET CLR 3.0.30618)',
            'Accept': 'text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8',
            'Accept-Language': 'en-US,en;q=0.5',
            'Accept-Encoding': 'gzip, deflate, br',
            'Connection': 'keep-alive'
        }
    ]
    
    # 设置HTTP代理
    proxies = {
        'http': 'http://127.0.0.1:8087',
        'https': 'http://127.0.0.1:8087',
    }

    def __init__(self, use_proxies=False):
        self.use_proxies = use_proxies
        self.session = requests.Session()

    def get_page(self, url):
        '''
        获得页面的内容，若请求不成功，则抛出 RequestPageError
        '''

        try:
            response = self.get_response(url)
        except (RequestPageError, PageNotFoundError) as e:
            raise e
        return response.text


    def get_response(self, url, stream=False):
        '''
        获得响应，若请求不成功，则抛出 RequestPageError
        '''

        if self.use_proxies:
            response = self.session.get(url=url, headers=random.choice(SimpleSpider.headers),
                                        proxies=SimpleSpider.proxies, verify=False, stream=stream)
        else:
            response = self.session.get(url=url, headers=random.choice(SimpleSpider.headers), 
                                        stream=stream)
        if response.status_code == requests.codes.not_found:
            raise PageNotFoundError('The page was not found: %s' % url)
        elif response.status_code != requests.codes.ok:
            self.session.cookies.clear()
            self.use_proxies = not self.use_proxies
            raise RequestPageError('Failed to get the content of page: %s' % url)

        return response


    def parse_page(self, url, parser):
        '''
        解析页面，若对页面的请求不成功，则抛出 RequestPageError
        '''

        try:
            page = self.get_page(url)
        except (RequestPageError, PageNotFoundError) as e:
            raise e
        soup = BeautifulSoup(page, 'lxml')
        results = parser(url, soup)
        return results


class ScrapeDoubanMovies:
    '''
    爬取豆瓣电影
    '''

    def __init__(self, start_url, score, logger, use_proxies=False):
        '''
        初始化
        '''

        self.ids_seen = set()
        self.start_url = start_url
        self.score = score
        self.logger = logger
        self.allow_score = score - 1.0
        self.spider = SimpleSpider(use_proxies=use_proxies)


    def start_scrape(self):
        '''
        启动爬虫
        '''

        self.scraping(self.start_url)


    def movie_page_parser(self, page_url, soup):
        '''
        解析电影页面，获得电影信息
        '''

        title = soup.find('h1').find('span', {'property': 'v:itemreviewed'}).string
        score = float(soup.find('strong', {'class': 'll rating_num'}).string)
        description = ' '.join(soup.find('span', {'property': 'v:summary'}).get_text().split())

        # 获得其它推荐电影的链接
        more_movies = soup.find('div', {'class': 'recommendations-bd'})
        if not more_movies:
            links = []
        else:
            links = [movie.get('href') for movie in more_movies.find_all('a', href=re.compile('^(https://movie.douban.com/subject)'))] 

        # 获得影评的链接
        comments = soup.find('div', {'id': 'review_section'}) 
        if not comments:
            comments_links = []
        else:
            comment_links = set(item.get('href') for item in comments.find_all('a', href=re.compile('^(https://movie.douban.com/review)')))

        poster_name = soup.find('img', {'rel': 'v:image'}).get('src').split('/')[-1]
        movie_info = {
            'id': self.extract_id(page_url), 
            'title': title, 
            'score': score, 
            'description': description, 
            'poster_name': poster_name,
            'links': links, 
            'comment_links': comment_links
        }
        return movie_info

       
    def is_movie_page_parser(self, page_url, soup):
        '''
        解析页面，判断是否为电影页面
        '''

        info = soup.find('div', {'id': 'info'}).find_all('span', {'class': 'pl'})
        info_tags = set(item.string for item in info)
        types = soup.find('div', {'id': 'info'}).find_all('span', {'property': 'v:genre'})
        type_tags = set(item.string for item in types)
        
        if ('片长:' in info_tags or '上映日期:' in info_tags) and ('纪录片' not in type_tags):
            return True
        return False


    def movie_comment_parser(self, page_url, soup):
        '''
        解析影评页面，获得影评
        '''

        title = soup.find('h1').find('span', {'property': 'v:summary'}).string
        author = soup.find('span', {'property': 'v:reviewer'}).string
        comment = soup.find('div', {'property': 'v:description'})
        comment_info = {
            'id': self.extract_id(page_url), 
            'title': title, 
            'author': author, 
            'comment': comment
        } 
        return comment_info
        

    def extract_id(self, page_url):
        '''
        根据 url 获得电影的 id，若 url 中不包含 id，则返回 -1
        '''

        for token in page_url.split('/'):
            if token.isdigit():
                return int(token)
        return -1


    def save_movie_and_comments(self, movie_info, comments):
        '''
        保存电影信息
        '''

        movie = Movie(movie_info['id'], movie_info['title'],
                      movie_info['score'], movie_info['description'])
        db.session.add(movie)

        for item in comments:
            comment = Comment(item['id'], item['title'], item['author'], item['comment'], movie=movie)
            db.session.add(comment)
        db.session.commit()


    def download_and_save_image(self, movie_info):
        '''
        下载并保存电影海报
        '''

        poster_url = 'https://img1.doubanio.com/view/photo/photo/public/' + movie_info['poster_name']
        saved_image_path = os.path.join(base_dir_path, 'app', 'static', 'image', str(movie_info['id']) + '.jpg')
        if os.path.isfile(saved_image_path):
            return
            
        while True:
            try:
                response = self.spider.get_response(poster_url, stream=True)
                with open(saved_image_path, 'wb') as out_file:
                    response.raw.decode_content = True
                    shutil.copyfileobj(response.raw, out_file)
                logger.info('Download image successfully for movie: %s' % movie_info['title'])
                return
            except RequestPageError as e:
                logger.error('Failed to download image for movie: %s, the url is %s' % (movie_info['title'], poster_url))
            except Exception as e:
                raise e


    def is_movie_page(self, page_url):
        '''
        判断是否为电影页面
        '''

        while True:
            try:
                is_movie = self.spider.parse_page(page_url, parser=self.is_movie_page_parser)
                return is_movie
            except RequestPageError as e:
                logger.error('Failed to test if the page is a movie page: %s' % page_url)
            except Exception as e:
                raise e


    def get_movie_info(self, page_url):
        '''
        获得电影的信息
        '''

        while True:
            try:
                movie_info = self.spider.parse_page(page_url, parser=self.movie_page_parser)
                logger.info('Get the infomation successfully for movie: %s' % movie_info['title'])
                return movie_info
            except RequestPageError as e:
                logger.error('Failed to get the infomation for movie: %s' % movie_info['title'])
            except Exception as e:
                raise e


    def get_movie_comments(self, movie_info):
        '''
        获得电影的评论
        '''

        comments = []
        for link in movie_info['comment_links']:
            while True:
                try:
                    comment_info = self.spider.parse_page(link, parser=self.movie_comment_parser)
                    comments.append(comment_info)
                    break
                except RequestPageError as e:
                    logger.error('Failed to get the comment for movie: %s' % movie_info['title'])
                except Exception as e:
                    raise e
        logger.info('Get all the comments successfully for movie: %s' % movie_info['title'])
        return comments


    def scraping(self, url):
        links = deque()
        links.append(url)

        while links:
            page_url = links.popleft()
            
            # 若当前页面已经爬取过，则不作处理
            if self.extract_id(page_url) in self.ids_seen:
                continue
                
            try:
                is_movie = self.is_movie_page(page_url)
                if not is_movie:
                    continue
                movie_info = self.get_movie_info(page_url)
            except PageNotFoundError as e:
                # self.ids_seen.add(self.extract_id(page_url))
                logger.error('The Movie page was not found on the server: %s' % page_url)
                continue
            except Exception as e:
                logger.error('Connection close by server')
                self.spider.session.cookies.clear()
                self.spider.use_proxies = not self.spider.use_proxies
                links.append(page_url)
                continue
            
            # 若当前电影评分过低，则不作处理
            if movie_info['score'] < self.allow_score:
                self.ids_seen.add(movie_info['id'])
                continue
            
            if movie_info['score'] < self.score:
                self.ids_seen.add(movie_info['id'])
                logger.info('Does not select movie: %s, score: %s' % (movie_info['title'], movie_info['score']))
                continue

            try:
                comments = self.get_movie_comments(movie_info)
                self.download_and_save_image(movie_info)
            except PageNotFoundError as e:
                # self.ids_seen.add(movie_info['id'])
                logger.error('Page was not found on the server: %s' % page_url)
                continue
            except Exception as e:
                logger.error('Connection close by server')
                self.spider.session.cookies.clear()                
                self.spider.use_proxies = not self.spider.use_proxies
                links.append(page_url)
                continue
                
            self.save_movie_and_comments(movie_info, comments)
            logger.info('Select movie: %s, score: %s' % (movie_info['title'], movie_info['score']))
            self.ids_seen.add(movie_info['id'])
            
            for link in movie_info['links']:
                if self.extract_id(link) not in self.ids_seen:
                    links.append(link)


def init_logger():
    '''
    初始化 logger
    '''

    logger = logging.getLogger('spider.py')
    log_file = os.path.join(base_dir_path, 'log', 'spider.log')
    log_file = 'log/spider.log'
    log_handler = logging.FileHandler(log_file, mode='w', encoding='utf-8')
    log_formatter = logging.Formatter('%(asctime)s [%(levelname)s] %(name)s: %(message)s')
    log_handler.setFormatter(log_formatter)
    logger.addHandler(log_handler)
    logger.propagate = False
    logger.setLevel(logging.INFO)
    return logger


if __name__ == '__main__':
    '''
    运行爬虫
    '''
    db.create_all()
    logger = init_logger()
    requests.packages.urllib3.disable_warnings(InsecureRequestWarning)
    spider = ScrapeDoubanMovies(start_url='https://movie.douban.com/subject/1292052/', 
                                score=8.5, logger=logger)
    spider.start_scrape()




# def get_douban_movie_types():
#    '''
#    得到豆瓣电影的分类，以及每个分类的链接
#    '''
#
#    url = 'https://movie.douban.com/tag/'
#    spider = SimpleSpider()
#
#    def douban_movie_types_parser(soup):
#        type_tags = soup.find('table', {'class': 'tagCol'})
#        return [(tag.string, tag.get('href')) for tag in type_tags.find_all('a')]
#
#    movie_types = spider.parse_page(url, parser=douban_movie_types_parser)
#    return movie_types
