from django.db import models

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
    title = models.CharField(max_length=30)
    description = models.TextField()
    screenshotlink = models.URLField()
    downloadlink = models.URLField()
    timelapselink = models.URLField()
    gamejam = models.ForeignKey('Jam')
