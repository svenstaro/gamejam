from models import *
from django import forms
from django.forms.widgets import RadioFieldRenderer

class StarRadioFieldRenderer(RadioFieldRenderer):
    def render(self):
        """Outputs a <ul> for this set of radio fields."""
        from django.utils.encoding import force_unicode
        from django.utils.safestring import mark_safe
        return mark_safe(u'<ul class="stars">\n%s\n</ul>' % u'\n'.join([u'<li>%s</li>'
                % force_unicode(w) for w in self]))

class StarRadioSelect(forms.RadioSelect):
    renderer = StarRadioFieldRenderer

class StarField(forms.TypedChoiceField):
    def __init__(self, *args, **kwargs):
        kwargs['choices'] = (
            #('0', "Terrible"),
            ('1', "Poor"),
            ('2', "Okay"),
            ('3', "Good"),
            ('4', "Great"),
            ('5', "Fantastic"),
        )
        kwargs['coerce'] = int
        kwargs['empty_value'] = 0
        kwargs['widget'] = StarRadioSelect
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
