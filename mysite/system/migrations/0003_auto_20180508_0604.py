# Generated by Django 2.0.4 on 2018-05-08 06:04

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('system', '0002_userprofile_field'),
    ]

    operations = [
        migrations.AlterField(
            model_name='userprofile',
            name='field',
            field=models.CharField(max_length=3),
        ),
    ]