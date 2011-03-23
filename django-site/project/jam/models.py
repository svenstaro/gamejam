import uuid
from django.db import models
from django.contrib.auth.models import User
from registration.signals import user_registered
from django.db.models.signals import post_save

class Announcement(models.Model):
    text = models.TextField()
    date = models.DateTimeField(auto_now=True)

    def __unicode__(self):
        return self.text

class Jam(models.Model):
    title = models.CharField(max_length=30)
    description = models.TextField()
    rules = models.TextField()
    redditpost = models.URLField()
    date_start = models.DateTimeField()
    date_end = models.DateTimeField()
    date_vote_end = models.DateTimeField()
    date_announced = models.DateTimeField(auto_now=True)

    def get_absolute_url(self):
        return "/jams/%s/" % self.pk

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

    def get_absolute_url(self):
        return "/jams/entry/%s/" % self.pk

    def __unicode__(self):
        return self.title

class UserProfile(models.Model):
    user = models.OneToOneField(User)
    uuid = models.CharField(max_length=32)
    can_vote = models.BooleanField()

    def __unicode__(self):
        return self.user.username

class Vote(models.Model):
    user = models.ForeignKey(User)
    entry = models.ForeignKey('Entry')
    couldnt_run = models.BooleanField()

class Rating(models.Model):
    vote = models.OneToOneField('Vote')
    gameplay = models.PositiveSmallIntegerField()
    graphics = models.PositiveSmallIntegerField()
    music = models.PositiveSmallIntegerField()
    fun = models.PositiveSmallIntegerField()
    overall = models.PositiveSmallIntegerField()

def add_user_profile(sender, instance, created, **kwargs):
    if created:
        print "adding user profile"
        a_uuid = uuid.uuid4()
        user_profile = UserProfile(user=instance, uuid=a_uuid.hex, can_vote=False)
        user_profile.save()

post_save.connect(add_user_profile, sender=User)
