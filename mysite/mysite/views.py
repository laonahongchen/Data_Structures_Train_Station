from django.shortcuts import render
from django.shortcuts import render_to_response
from django.http import HttpResponse
from django.template import loader
# Create your views here.

context = {'login_name':'test', 'authority':0}
#need to fix context which send login_name and authority to html
#authority 0: not login 1: normal user 2:admin 3: super admin

def index(request):
    return render(request, 'index.html', context)

def about(request):
    return render_to_response('about.html', context)

def login(request):
    return render_to_response('login.html', context)
    #get id='inputName' with password ='inputPassword'
    #maybe get remember-me so use cookies
    
def signup(request):
    return render_to_response('signup.html', context)
