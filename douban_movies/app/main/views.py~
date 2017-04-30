from flask import render_template, abort, request
from app.models import Movie, Comment
from app.main import main

@main.route('/', methods=['GET'])
def index():
    '''
    显示首页
    '''
    
    page = request.args.get('page', 1, type=int)
    pagination = Movie.query.order_by(Movie.score.desc()).paginate(page, 10, error_out=False)
    movies = pagination.items
    return render_template('index.html', movies=movies, pagination=pagination)
  
    
@main.route('/articles/<int:id>')
def movie(id):
    '''
    显示电影详情页面
    '''

    movie_info = Movie.query.filter_by(id=id).first()
    if not movie_info:
        abort(404)
    poster_path = 'image/' + str(movie_info.id) + '.jpg'
    return render_template('text.html', movie_info=movie_info, poster_path=poster_path)
    
