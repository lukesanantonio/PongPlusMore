import io
import sys
import json
import random
import time
import signal

class Vec:
    def __init__(self, x = 0.0, y = 0.0):
        self.x = x
        self.y = y

def dump_vec(vec):
    json = {}
    json['x'] = vec.x
    json['y'] = vec.y
    return json

def parse_vec(json):
    return Vec(json['x'], json['y'])

class Volume:
    def __init__(self, pos = Vec(), width = 0.0, height = 0.0):
        self.pos = pos
        self.width = width
        self.height = height

def dump_volume(vol):
    json = {}
    json['Position'] = dump_vec(vol.pos)
    json['Width'] = vol.width
    json['Height'] = vol.height
    return json

def parse_volume(json):
    return Volume(parse_vec(json['Position']), json['Width'], json['Height'])

class PhysicsOptions:
    def __init__(self):
        self.destination = Vec(0.0, 0.0)
        self.constraints = 0

def dump_physics(phys):
    json = {}

    if hasattr(phys, 'velocity'):
        json['Velocity'] = dump_vec(phys.velocity)
    elif hasattr(phys, 'destination'):
        json['Destination'] = dump_vec(phys.destination)

    # Don't put constraints in the object for the moment.

    return json

def parse_physics(json):
    phys = PhysicsOptions()
    del phys.destination
    if 'Velocity' in json:
        phys.velocity = parse_vec(json['Velocity'])
    elif 'Destination' in json:
        phys.destination = parse_vec(json['Destination'])
    return phys

class Object:
    def __init__(self, vol = Volume(), phys_opt = PhysicsOptions()):
        self.volume = vol
        self.physics_options = phys_opt

def dump_object(obj):
    json = {}

    json['Volume'] = dump_volume(obj.volume)
    json['PhysicsOptions'] = dump_physics(obj.physics_options)

    return json

def parse_object(json):
    return Object(parse_volume(json['Volume']),
                  parse_physics(json['PhysicsOptions']))


class Action:
    def __init__(self, action_id, method):
        self.action_id = action_id
        self.method = method

def dump_action(action):
    json = {}

    json['id'] = action.action_id
    json['method'] = action.method

    return json

class ObjectCreationAction(Action):
    def __init__(self, action_id):
        super().__init__(action_id, 'Server.CreateObject')
        self.obj = Object()

def dump_create_action(action):
    json = dump_action(action)
    json['params'] = [dump_object(action.obj)]
    return json

class ObjectDeletionAction(Action):
    def __init__(self, action_id):
        super().__init__(action_id, 'Server.DeleteObject')
        self.obj_id = 0

def dump_delete_action(action):
    json = dump_action(action)
    json['params'] = [action.obj_id]
    return json

class QueryObjectRequest(Action):
    def __init__(self, action_id):
        super().__init__(action_id, 'Server.QueryObject')
        self.obj_id = 0

def dump_query_request(action):
    json = dump_action(action)
    json['params'] = [action.obj_id]
    return json

def wait_for_header(fd):
    header = fd.read(3)
    if header != 'PpM':
        sys.exit(2)

if __name__ == '__main__':
    wait_for_header(sys.stdin)

    time.sleep(1)
    action = ObjectCreationAction(1)
    action.obj.volume.pos.x = 500;
    action.obj.volume.pos.y = 500;
    action.obj.volume.width = 20;
    action.obj.volume.height = 20;
    action.obj.physics_options.destination = action.obj.volume.pos;
    print(json.dumps(dump_create_action(action)));
