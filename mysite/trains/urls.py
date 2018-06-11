
from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='train_add'),
    path('o/', views.index1, name='train_add_1'),
    path('t/', views.index2, name='train_add_2'),
    path('query_train/', views.query_train, name='qt'),
]
