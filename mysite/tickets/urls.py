
from django.urls import path

from . import views

urlpatterns = [
    path('', views.index, name='train'),
    path('addTrain/', views.addTrain, name='add_train'),
]
