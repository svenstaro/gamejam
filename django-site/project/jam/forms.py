from models import *
from django import forms

class StarField(forms.TypedChoiceField):
    def __init__(self, *args, **kwargs):
        kwargs['choices'] = (
            ('0', "Terrible"),
            ('1', "Poor"),
            ('2', "Okay"),
            ('3', "Good"),
            ('4', "Great"),
            ('5', "Fantastic"),
        )
        kwargs['coerce'] = int
        kwargs['empty_value'] = 0
        kwargs['widget'] = forms.RadioSelect
        super(self.__class__, self).__init__(*args, **kwargs)

class RatingForm(forms.ModelForm):
    gameplay = StarField()
    graphics = StarField()
    music = StarField()
    fun = StarField()
    overall = StarField()

    class Meta:
        model = Rating
        exclude = ('vote',)

class EntryForm(forms.ModelForm):
    class Meta:
        model = Entry
        exclude = ('user', 'gamejam')
