# this allows you to allow easily an integrator for binding mqtt
# objects together
#
# was initially developed for ulnoiot (http://github.com/ulno/ulnoiot)
#
# Author: Ulrich Norbisrath (http://ulno.net)
# Created: 2017-10-05

from integriot import devices

_stopped = False
_main_devices = None


def sensor(t, cb={}):
    global _main_devices
    return _main_devices.sensor(t, cb)


def actor(t):
    global _main_devices
    return _main_devices.actor(t)


def device_list():
    global _main_devices
    return _main_devices.device_list


def switch(t, on="on", off="off", set="set", state=""):
    global _main_devices
    return _main_devices.switch(t,on,off,set,state)
led = switch
relay = switch


def init(mqtt_host):
    """
    Just init one mqtt connection and its devices.
    :param mqtt_host:
    :return:
    """
    global _main_devices
    _main_devices = devices.Devices(mqtt_host)   # TODO: auth+tls


def prefix(p):
    global _main_devices
    _main_devices.topic_prefix = p


def stop():
    global _stopped
    _stopped = True


def start(*args):
    """
    Run until interrupted or quit called from any of the callbacks.

    :param args: a list of Devices-objects (if empty try to use global devices)
    :return:
    """
    global _main_devices

    if len(args) == 0:
        args = [_main_devices]

    while not _stopped:
        for d in args:
            d.client.loop(0.01)


run = start
