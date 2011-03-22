import uuid
from django.db import models
from django.contrib.auth.models import User
from registration.signals import user_registered

class Announcement(models.Model):
    text = models.TextField()

    def __unicode__(self):
        return self.text

class Jam(models.Model):
    title = models.CharField(max_length=30)
    description = models.TextField()
    rules = models.TextField()
    redditpost = models.URLField()
    date_start = models.DateTimeField()
    date_end = models.DateTimeField()
    date_announced = models.DateTimeField(auto_now=True)

    def __unicode__(self):
        return self.title

class Entry(models.Model):
    user = models.ForeignKey(User)
    title = models.CharField(max_length=30)
    description = models.TextField()
    screenshotlink = models.URLField()
    downloadlink = models.URLField()
    timelapselink = models.URLField()
    gamejam = models.ForeignKey('Jam')

class UserProfile(models.Model):
    user = models.OneToOneField(User)
    uuid = models.CharField(max_length=32)

def add_user_profile(sender, user, request, **kwargs):
    print "adding user profile"
    a_uuid = uuid.uuid4()
    user_profile = UserProfile(user=user, uuid=a_uuid.hex)
    user_profile.save()

user_registered.connect(add_user_profile)
