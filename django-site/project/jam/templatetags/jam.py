from gamejam.jam.models import Jam
from django import template
import math

register = template.Library()

@register.inclusion_tag('jams.html', takes_context=True)
def show_jams(context):
    curjam = None
    if 'entry' in context:
        curjam = context['entry'].gamejam
    if 'jam' in context:
        curjam = context['jam']
    return {
        'jams': Jam.objects.all(),
        'curjam': curjam,
    }

@register.inclusion_tag('stars.html')
def stars(num_stars):
    print type(num_stars), num_stars
    fraction_star = num_stars != int(num_stars)
    on_stars = int(num_stars)
    off_stars = int(5 - num_stars)
    print on_stars, off_stars
    fraction = {}
    if fraction_star:
        fraction['on'] = math.floor((num_stars - int(num_stars)) * 16)
        fraction['off'] = 16 - fraction['on']
    return {
        'on_stars': range(on_stars),
        'off_stars': range(off_stars),
        'fraction_star': fraction_star,
        'fraction': fraction,
    }

@register.simple_tag
def current(request, pattern):
    import re
    if re.search(pattern, request.path):
        return 'current'
    return ''
