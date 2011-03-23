from models import *
from django import forms

class RatingForm(forms.ModelForm):
    gameplay = forms.IntegerField(min_value=0, max_value=5)
    graphics = forms.IntegerField(min_value=0, max_value=5)
    music = forms.IntegerField(min_value=0, max_value=5)
    fun = forms.IntegerField(min_value=0, max_value=5)
    overall = forms.IntegerField(min_value=0, max_value=5)

    class Meta:
        model = Rating
        exclude = ('vote',)
