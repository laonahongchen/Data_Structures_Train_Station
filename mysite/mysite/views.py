from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse,HttpResponseRedirect
from django.template import loader
from django.urls import reverse
# Create your views here.

context = {'login_name':'test', 'authority':0, 'style':1}
#need to fix context which send login_name and authority to html
#authority 0: not login 1: normal user 2:admin

def cstyle0(request):
    context['style'] = 0
    return HttpResponseRedirect(reverse('index'))

def cstyle1(request):
    context['style'] = 1
    return HttpResponseRedirect(reverse('index'))

def index(request):
    return render(request, 'index.html', context)

def about(request):
    return render_to_response('About.html', context)

def login(request):
    return render_to_response('Login.html', context)
    #get id='inputName' with password ='inputPassword'
    #maybe get remember-me so use cookies please

def signup(request):
    return render_to_response('Signup.html', context)

def cinfo(request):
    return render_to_response('Signup.html', context)
