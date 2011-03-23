from gamejam.jam.models import Jam
from django import template
import math

register = template.Library()

@register.inclusion_tag('jams.html')
def show_jams():
    return {
        'jams': Jam.objects.all()
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
