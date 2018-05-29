
from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='train_seek'),
    path('buyhistory/', views.buy_history, name="buy_history"),
]
