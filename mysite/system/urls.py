from django.urls import path, include

from . import views

urlpatterns = [
    path('', views.index, name='cr_index'),
    path('register/', views.register, name='cr_register'),
    path('login/', views.user_login, name='cr_login'),
    path('logout/', views.user_logout, name='cr_logout'),
    path('booking/', views.booking, name='cr_booking'),
]