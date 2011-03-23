from gamejam.jam.models import Jam
from django import template

register = template.Library()

@register.inclusion_tag('jams.html')
def show_jams():
    return {
        'jams': Jam.objects.all()
    }
