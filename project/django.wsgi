import os
import sys

import django.core.handlers.wsgi
application = django.core.handlers.wsgi.WSGIHandler()

path = '/var/django'
if path not in sys.path:
    sys.path.append(path)

os.environ['DJANGO_SETTINGS_MODULE'] = 'gamejam.settings'
