/*
 * ***** BEGIN GPL LICENSE BLOCK *****
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 * The Original Code is Copyright (C) 2001-2002 by NaN Holding BV.
 * All rights reserved.
 *
 * The Original Code is: all of this file.
 *
 * Contributor(s): none yet.
 *
 * ***** END GPL LICENSE BLOCK *****
 */

#include "RAS_StorageVBO.h"
#include "RAS_MeshObject.h"

#include "glew-mx.h"

VBO::VBO(RAS_DisplayArray *data, unsigned int indices)
{
	this->data = data;
	this->size = data->m_vertex.size();
	this->indices = indices;
	this->stride = sizeof(RAS_TexVert);

	//	Determine drawmode
	if (data->m_type == data->QUAD)
		this->mode = GL_QUADS;
	else if (data->m_type == data->TRIANGLE)
		this->mode = GL_TRIANGLES;
	else
		this->mode = GL_LINE;

	// Generate Buffers
	glGenBuffers(1, &this->ibo);
	glGenBuffers(1, &this->vbo_id);

	// Fill the buffers with initial data
	UpdateIndices();
	UpdateData();

	// Establish offsets
	this->vertex_offset = (void*)(((RAS_TexVert*)0)->getXYZ());
	this->normal_offset = (void*)(((RAS_TexVert*)0)->getNormal());
	this->tangent_offset = (void*)(((RAS_TexVert*)0)->getTangent());
	this->color_offset = (void*)(((RAS_TexVert*)0)->getRGBA());
	this->uv_offset = (void*)(((RAS_TexVert*)0)->getUV(0));
}

VBO::~VBO()
{
	glDeleteBuffers(1, &this->ibo);
	glDeleteBuffers(1, &this->vbo_id);
}

void VBO::UpdateData()
{
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);
	glBufferData(GL_ARRAY_BUFFER, this->stride*this->size, &this->data->m_vertex[0], GL_STATIC_DRAW);
}

void VBO::UpdateIndices()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, data->m_index.size() * sizeof(GLushort),
					&data->m_index[0], GL_STATIC_DRAW);
}

void VBO::Draw(int texco_num, RAS_IRasterizer::TexCoGen* texco, int attrib_num, RAS_IRasterizer::TexCoGen* attrib, int *attrib_layer, bool multi)
{
	int unit;

	// Bind buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ibo);
	glBindBuffer(GL_ARRAY_BUFFER, this->vbo_id);

	// Vertexes
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, this->stride, this->vertex_offset);

	// Normals
	glEnableClientState(GL_NORMAL_ARRAY);
	glNormalPointer(GL_FLOAT, this->stride, this->normal_offset);

	// Colors
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, this->stride, this->color_offset);

	if (MX_multitexture) {
		if (multi)
		{
			for (unit = 0; unit < texco_num; ++unit)
			{
				glClientActiveTexture(GL_TEXTURE0 + unit);
				switch (texco[unit]) {
					case RAS_IRasterizer::RAS_TEXCO_ORCO:
					case RAS_IRasterizer::RAS_TEXCO_GLOB:
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(3, GL_FLOAT, this->stride, this->vertex_offset);
						break;
					case RAS_IRasterizer::RAS_TEXCO_UV:
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(2, GL_FLOAT, this->stride, (void*)((intptr_t)this->uv_offset+(sizeof(GLfloat)*2*unit)));
						break;
					case RAS_IRasterizer::RAS_TEXCO_NORM:
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(3, GL_FLOAT, this->stride, this->normal_offset);
						break;
					case RAS_IRasterizer::RAS_TEXTANGENT:
						glEnableClientState(GL_TEXTURE_COORD_ARRAY);
						glTexCoordPointer(4, GL_FLOAT, this->stride, this->tangent_offset);
						break;
					default:
						break;
				}
			}
			glClientActiveTexture(GL_TEXTURE0);
		}
		else //TexFace
		{
			glClientActiveTexture(GL_TEXTURE0);
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glTexCoordPointer(2, GL_FLOAT, this->stride, this->uv_offset);
		}
	}

	if (MX_vertexattrib)
	{
		for (unit = 0; unit < attrib_num; ++unit)
		{
			switch (attrib[unit]) {
				case RAS_IRasterizer::RAS_TEXCO_ORCO:
				case RAS_IRasterizer::RAS_TEXCO_GLOB:
					glVertexAttribPointer(unit, 3, GL_FLOAT, GL_FALSE, this->stride, this->vertex_offset);
					glEnableVertexAttribArray(unit);
					break;
				case RAS_IRasterizer::RAS_TEXCO_UV:
					glVertexAttribPointer(unit, 2, GL_FLOAT, GL_FALSE, this->stride, (void*)((intptr_t)this->uv_offset+attrib_layer[unit]*sizeof(GLfloat)*2));
					glEnableVertexAttribArray(unit);
					break;
				case RAS_IRasterizer::RAS_TEXCO_NORM:
					glVertexAttribPointer(unit, 2, GL_FLOAT, GL_FALSE, stride, this->normal_offset);
					glEnableVertexAttribArray(unit);
					break;
				case RAS_IRasterizer::RAS_TEXTANGENT:
					glVertexAttribPointer(unit, 4, GL_FLOAT, GL_FALSE, this->stride, this->tangent_offset);
					glEnableVertexAttribArray(unit);
					break;
				default:
					break;
			}
		}
	}
	
	glDrawElements(this->mode, this->indices, GL_UNSIGNED_SHORT, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (MX_vertexattrib)
	{
		for (int i = 0; i < attrib_num; ++i)
			glDisableVertexAttribArray(i);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

RAS_StorageVBO::RAS_StorageVBO(int *texco_num, RAS_IRasterizer::TexCoGen *texco, int *attrib_num, RAS_IRasterizer::TexCoGen *attrib, int *attrib_layer):
	m_drawingmode(RAS_IRasterizer::KX_TEXTURED),
	m_texco_num(texco_num),
	m_attrib_num(attrib_num),
	m_texco(texco),
	m_attrib(attrib),
	m_attrib_layer(attrib_layer)
{
}

RAS_StorageVBO::~RAS_StorageVBO()
{
}

bool RAS_StorageVBO::Init()
{
	return true;
}

void RAS_StorageVBO::Exit()
{
	m_vbo_lookup.clear();
}

void RAS_StorageVBO::IndexPrimitives(RAS_MeshSlot& ms)
{
	IndexPrimitivesInternal(ms, false);
}

void RAS_StorageVBO::IndexPrimitivesMulti(RAS_MeshSlot& ms)
{
	IndexPrimitivesInternal(ms, true);
}

void RAS_StorageVBO::IndexPrimitivesInternal(RAS_MeshSlot& ms, bool multi)
{
	RAS_MeshSlot::iterator it;
	VBO *vbo;

	for (ms.begin(it); !ms.end(it); ms.next(it))
	{
		vbo = m_vbo_lookup[it.array];

		if (vbo == 0)
			m_vbo_lookup[it.array] = vbo = new VBO(it.array, it.totindex);

		// Update the vbo
		if (ms.m_mesh->MeshModified())
		{
			vbo->UpdateData();
		}

		vbo->Draw(*m_texco_num, m_texco, *m_attrib_num, m_attrib, m_attrib_layer, multi);
	}
}
