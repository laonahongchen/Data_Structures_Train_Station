from django import forms
from django.contrib.auth.models import User 
from registration.forms import RegistrationFormUniqueEmail
from system.models import UserProfile

class UserForm(forms.ModelForm):
    password = forms.CharField(widget=forms.PasswordInput())

    class Meta:
        model = User 
        fields = ('username', 'email', 'password')

class UserProfileForm(forms.ModelForm):
    class Meta:
        model = UserProfile
        #addtional
        fields = ()