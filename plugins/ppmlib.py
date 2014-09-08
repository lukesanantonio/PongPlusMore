import io
import sys
import json

class Vec:
    def __init__(self, x = 0.0, y = 0.0):
        self.x = x
        self.y = y

def dump_vec(vec):
    json = {}
    json['x'] = vec.x
    json['y'] = vec.y
    return json

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

class Object:
    def __init__(self, vol = Volume(), phys_opt = PhysicsOptions()):
        self.volume = vol
        self.physics_options = phys_opt

def dump_object(obj):
    json = {}

    json['Volume'] = dump_volume(obj.volume)
    json['PhysicsOptions'] = dump_physics(obj.physics_options)

    return json

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

def wait_for_header(fd):
    header = fd.read(3)
    if header != 'PpM':
        sys.exit(2)

if __name__ == '__main__':
    wait_for_header(sys.stdin)

    action = ObjectCreationAction(1)
    action.obj.volume.pos = Vec(425, 425)
    action.obj.volume.width = 150
    action.obj.volume.height = 150
    action.obj.physics_options.destination = action.obj.volume.pos

    print(json.dumps(dump_create_action(action)))

    f = open('out', 'w')
    f.write(sys.stdin.readline())
