#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# author: senlinzhan@gmail.com
#

from app import db

class Movie(db.Model):
    '''
    电影的信息
    '''

    __tablename__ = 'movies'
    
    id = db.Column(db.Integer, primary_key=True, nullable=False)
    title = db.Column(db.String(100), nullable=False)
    score = db.Column(db.Float, nullable=False)
    description = db.Column(db.Text, nullable=False)
    comments = db.relationship('Comment', backref='movie')

    def __init__(self, id, title, score, description):
        self.id = id
        self.title = title
        self.score = score
        self.description = description

    def __repr__(self):
        return '<id: %r, title: %r, score: %r, description: %r>' % (self.id, self.title, self.score, self.description)

        
class Comment(db.Model):
    '''
    电影评论
    '''

    __tablename__ = 'comments'
    
    id = db.Column(db.Integer, primary_key=True, nullable=False)
    title = db.Column(db.String(100), nullable=False)
    author = db.Column(db.String(24), nullable=False)
    comment = db.Column(db.Text, nullable=False)
    movie_id = db.Column(db.Integer, db.ForeignKey('movies.id'))

    def __init__(self, id, title, author, comment, movie):
        self.id = id
        self.title = title
        self.author = author
        self.comment = comment
        self.movie = movie

    def __repr__(self):
        return '<id: %r, movie_id: %r, comment: %r>' % (self.id, self.movie_id, self.comment)
    
