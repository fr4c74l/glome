#!BPY
# -*- coding: utf-8 -*-
#
# Making with 2 tabs indentation

"""
Name: 'Navigna Mesh Exporter'
Blender: 257
Group: 'Export'
Tooltip: 'Export meshes to Binary file format for Navigna Game Renega Desruga'
"""

# Copyright (C) 2011 Fractal Corp.
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

try:
	import bpy
	from bpy import *
except ImportError:
	sys.exit('Only works from within Blender!\n')

import os
import struct

#TODO: Change to relative path in Blender (bpy.path.relpath(path))
export_path = bpy.path.abspath('/tmp/')

#FIXME: Acochambration here! for display name of objects
def export_mesh(obj, data):
	try:
		print('Exporting ' + obj.name + '.mesh in ' + export_path + ' directory')
		bfile = open(export_path + '/' + obj.name + '.mesh', 'wb')
		bfile.write(struct.pack('<H', len(data.vertices)))
		for v in data.vertices:
			bfile.write(struct.pack('<f', v.co.x))
			bfile.write(struct.pack('<f', v.co.y))
			bfile.write(struct.pack('<f', v.co.z))
		bfile.write(struct.pack('<H', len(data.edges)))
		for e in data.edges:
			bfile.write(struct.pack('<H', e.key[0]))
			bfile.write(struct.pack('<H', e.key[1]))
		bfile.close()
	except:
		pass

# Only for testing
def read_file(obj, data):
	try:
		print('Reading ' + obj.name + '.mesh from ' + export_path + ' directory')
		bfile = open(export_path + '/' + obj.name + '.mesh', 'rb')
		fsize = struct.calcsize('f')
		isize = struct.calcsize('H')
		nv = bfile.read(isize)
		print(struct.unpack('<H', nv)[0])
		for v in data.vertices:
			x = bfile.read(fsize)
			y = bfile.read(fsize)
			z = bfile.read(fsize)
			print(struct.unpack('<f', x)[0], struct.unpack('<f', y)[0], struct.unpack('<f', z)[0])
		ne = bfile.read(isize)
		print(struct.unpack('<H', ne)[0])
		for e in data.edges:
			e0 = bfile.read(isize)
			e1 = bfile.read(isize)
			print(struct.unpack('<H', e0)[0], struct.unpack('<H', e1)[0])
		bfile.close()
	except:
		pass

#########
# Begin #
#########

class ExportMeshNavigna(bpy.types.Operator):
	bl_idname = "export.navigna"
	bl_label = "Export Mesh to Navigna Game"

	def execute(self, context):
		scene = context.scene
		#FIXME: Maybe (objs = scene.objects) is better, but i dont know how to take selected objects in the scene, only in the context
		objs = context.selected_objects
		for o in objs:
			data = o.to_mesh(scene,True,'PREVIEW')
			export_mesh(o, data)
			read_file(o,data)
		self.report({'INFO'}, "Done.")
		return {'FINISHED'}

# registering and menu integration
def register():
	bpy.utils.register_class(ExportMeshNavigna)

# unregistering and removing menus
def unregister():
	bpy.utils.unregister_class(ExportMeshNavigna)

if __name__ == "__main__":
	register()

