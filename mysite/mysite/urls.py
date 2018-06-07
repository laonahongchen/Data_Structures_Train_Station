"""mysite URL Configuration

The `urlpatterns` list routes URLs to views. For more information please see:
    https://docs.djangoproject.com/en/2.0/topics/http/urls/
Examples:
Function views
    1. Add an import:  from my_app import views
    2. Add a URL to urlpatterns:  path('', views.home, name='home')
Class-based views
    1. Add an import:  from other_app.views import Home
    2. Add a URL to urlpatterns:  path('', Home.as_view(), name='home')
Including another URLconf
    1. Import the include() function: from django.urls import include, path
    2. Add a URL to urlpatterns:  path('blog/', include('blog.urls'))
"""
from django.contrib import admin
from django.urls import path, include
from . import views

from django.conf.urls import handler404, handler500

urlpatterns = [
    path('admin/', admin.site.urls),
    path('', views.index, name='index'),
    path('about/', views.about, name='about'),
    path('uploading/', views.uploading, name='upload'),
    path('login/', views.login, name='login'),
    path('logout/', views.user_logout, name='logout'),
    path('signup/', views.signup, name='signup'),
    path('signup_for_admin/', views.signupadmin, name='signupadmin'),
    path('change_info/', views.cinfo, name="cinfo"),
    path('cstyle0/', views.cstyle0, name="cstyle0"),
    path('cstyle1/', views.cstyle1, name="cstyle1"),
    path('trains/', include('trains.urls')),
    path('tickets/', include('tickets.urls')),
    path('chinarailway/', include('system.urls')),
]

#handler404 = views.page_not_found
#handler500 = views.page_error
