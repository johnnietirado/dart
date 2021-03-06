/*
 * Copyright (c) 2011-2015, Georgia Tech Research Corporation
 * All rights reserved.
 *
 * Author(s): Jie (Jay) Tan <jtan34@cc.gatech.edu>
 *
 * Georgia Tech Graphics Lab and Humanoid Robotics Lab
 *
 * Directed by Prof. C. Karen Liu and Prof. Mike Stilman
 * <karenliu@cc.gatech.edu> <mstilman@cc.gatech.edu>
 *
 * This file is provided under the following "BSD-style" License:
 *   Redistribution and use in source and binary forms, with or
 *   without modification, are permitted provided that the following
 *   conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above
 *     copyright notice, this list of conditions and the following
 *     disclaimer in the documentation and/or other materials provided
 *     with the distribution.
 *   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 *   CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 *   INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 *   MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 *   DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 *   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 *   SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 *   LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF
 *   USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 *   AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 *   LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 *   ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 *   POSSIBILITY OF SUCH DAMAGE.
 */

#include <iostream>
#include <assimp/cimport.h>

#include "dart/common/Console.h"
#include "dart/dynamics/Skeleton.h"
#include "dart/dynamics/BodyNode.h"
#include "dart/dynamics/Shape.h"
#include "dart/dynamics/BoxShape.h"
#include "dart/dynamics/CylinderShape.h"
#include "dart/dynamics/EllipsoidShape.h"
#include "dart/dynamics/MeshShape.h"
#include "dart/dynamics/LineSegmentShape.h"
#include "dart/renderer/LoadOpengl.h"
#include "dart/renderer/OpenGLRenderInterface.h"

// Code taken from glut/lib/glut_shapes.c
static GLUquadricObj *quadObj;

#define QUAD_OBJ_INIT { if(!quadObj) initQuadObj(); }

static void initQuadObj(void)
{
    quadObj = gluNewQuadric();
    if(!quadObj)
        // DART modified error output
        std::cerr << "OpenGL: Fatal Error in DART: out of memory." << std::endl;
}
//glut/lib/glut_shapes.c

namespace dart {
namespace renderer {

void OpenGLRenderInterface::initialize() {
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glCullFace(GL_FRONT);
    glDisable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    //glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glShadeModel(GL_SMOOTH);
    clear(Eigen::Vector3d(1.0, 1.0, 1.0));
}

void OpenGLRenderInterface::destroy() {

}

void OpenGLRenderInterface::setViewport(int _x,int _y,int _width,int _height) {
    glViewport(_x, _y, _width, _height);
    mViewportX = _x;
    mViewportY = _y;
    mViewportWidth = _width;
    mViewportHeight = _height;
}

void OpenGLRenderInterface::getViewport(int& _x, int& _y, int& _width, int& _height) const {
    _x = mViewportX;
    _y = mViewportY;
    _width = mViewportWidth;
    _height =mViewportHeight;
}

void OpenGLRenderInterface::clear(const Eigen::Vector3d& _color) {
    glClearColor((GLfloat)_color[0], (GLfloat)_color[1], (GLfloat)_color[2], 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
}

void OpenGLRenderInterface::setDefaultLight() {

}

void OpenGLRenderInterface::turnLightsOff() {
    glDisable(GL_LIGHTING);
}

void OpenGLRenderInterface::turnLightsOn() {
    //not finished yet
    glEnable(GL_LIGHTING);
}

void OpenGLRenderInterface::setMaterial(const Eigen::Vector3d& _diffuse, const Eigen::Vector3d& _specular, double _cosinePow) {

}

void OpenGLRenderInterface::getMaterial(Eigen::Vector3d& _diffuse, Eigen::Vector3d& _specular, double& _cosinePow) const {

}

void OpenGLRenderInterface::setDefaultMaterial() {

}

void OpenGLRenderInterface::pushMatrix() {
    glPushMatrix();
}

void OpenGLRenderInterface::popMatrix() {
    glPopMatrix();
}

void OpenGLRenderInterface::pushName(int _id) {
    glPushName(_id);
}

void OpenGLRenderInterface::popName() {
    glPopName();
}

void OpenGLRenderInterface::translate(const Eigen::Vector3d& _offset) {
    glTranslated(_offset[0], _offset[1], _offset[2]);
}

void OpenGLRenderInterface::rotate(const Eigen::Vector3d& _axis, double _rad) {
    glRotated(_rad, _axis[0], _axis[1], _axis[2]);
}

void OpenGLRenderInterface::transform(const Eigen::Isometry3d& _transform) {
    glMultMatrixd(_transform.data());
}

void OpenGLRenderInterface::scale(const Eigen::Vector3d& _scale) {
    glScaled(_scale[0], _scale[1], _scale[2]);
}

void OpenGLRenderInterface::drawEllipsoid(const Eigen::Vector3d& _size) {
    glScaled(_size(0), _size(1), _size(2));

    GLdouble radius = 0.5;
    GLint slices = 16;
    GLint stacks = 16;

    // Code taken from glut/lib/glut_shapes.c
    QUAD_OBJ_INIT;
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    //gluQuadricTexture(quadObj, GL_TRUE);

    gluSphere(quadObj, radius, slices, stacks);
    //glut/lib/glut_shapes.c
}

void OpenGLRenderInterface::drawCube(const Eigen::Vector3d& _size) {
    glScaled(_size(0), _size(1), _size(2));

    // Code taken from glut/lib/glut_shapes.c
    static GLfloat n[6][3] =
    {
        {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0},
        {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0},
        {0.0, 0.0, 1.0},
        {0.0, 0.0, -1.0}
    };
    static GLint faces[6][4] =
    {
        {0, 1, 2, 3},
        {3, 2, 6, 7},
        {7, 6, 5, 4},
        {4, 5, 1, 0},
        {5, 6, 2, 1},
        {7, 4, 0, 3}
    };
    GLfloat v[8][3];
    GLint i;
    GLfloat size = 1;

    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -size / 2;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = size / 2;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -size / 2;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = size / 2;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = -size / 2;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = size / 2;

    for (i = 5; i >= 0; i--) {
        glBegin(GL_QUADS);
        glNormal3fv(&n[i][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glEnd();
    }
    //glut/lib/glut_shapes.c
}

void OpenGLRenderInterface::drawCylinder(double _radius, double _height) {
    glScaled(_radius, _radius, _height);

    GLdouble radius = 1;
    GLdouble height = 1;
    GLint slices = 16;
    GLint stacks = 16;

    // Graphics assumes Cylinder is centered at CoM
    // gluCylinder places base at z = 0 and top at z = height
    glTranslated(0.0, 0.0, -0.5);

    // Code taken from glut/lib/glut_shapes.c
    QUAD_OBJ_INIT;
    gluQuadricDrawStyle(quadObj, GLU_FILL);
    gluQuadricNormals(quadObj, GLU_SMOOTH);
    //gluQuadricTexture(quadObj, GL_TRUE);

    gluCylinder(quadObj, radius, radius, height, slices, stacks); //glut/lib/glut_shapes.c
    gluDisk(quadObj, 0, radius, slices, stacks);
    glTranslated(0.0,0.0,1.0);
    gluDisk(quadObj, 0, radius, slices, stacks);
}

void OpenGLRenderInterface::color4_to_float4(const aiColor4D *c, float f[4])
{
    f[0] = c->r;
    f[1] = c->g;
    f[2] = c->b;
    f[3] = c->a;
}

void OpenGLRenderInterface::set_float4(float f[4], float a, float b, float c, float d)
{
    f[0] = a;
    f[1] = b;
    f[2] = c;
    f[3] = d;
}

// This function is taken from the examples coming with assimp
void OpenGLRenderInterface::applyMaterial(const struct aiMaterial *mtl)
{
    float c[4];

    GLenum fill_mode;
    int ret1, ret2;
    aiColor4D diffuse;
    aiColor4D specular;
    aiColor4D ambient;
    aiColor4D emission;
    float shininess, strength;
    int two_sided;
    int wireframe;
    unsigned int max;

    set_float4(c, 0.8f, 0.8f, 0.8f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_DIFFUSE, &diffuse))
        color4_to_float4(&diffuse, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, c);

    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_SPECULAR, &specular))
        color4_to_float4(&specular, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);

    set_float4(c, 0.2f, 0.2f, 0.2f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_AMBIENT, &ambient))
        color4_to_float4(&ambient, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, c);

    set_float4(c, 0.0f, 0.0f, 0.0f, 1.0f);
    if(AI_SUCCESS == aiGetMaterialColor(mtl, AI_MATKEY_COLOR_EMISSIVE, &emission))
        color4_to_float4(&emission, c);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, c);

    max = 1;
    ret1 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS, &shininess, &max);
    if(ret1 == AI_SUCCESS) {
        max = 1;
        ret2 = aiGetMaterialFloatArray(mtl, AI_MATKEY_SHININESS_STRENGTH, &strength, &max);
        if(ret2 == AI_SUCCESS)
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess * strength);
        else
            glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
    }
    else {
        glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
        set_float4(c, 0.0f, 0.0f, 0.0f, 0.0f);
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, c);
    }

    max = 1;
    if(AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_ENABLE_WIREFRAME, &wireframe, &max))
        fill_mode = wireframe ? GL_LINE : GL_FILL;
    else
        fill_mode = GL_FILL;
    glPolygonMode(GL_FRONT_AND_BACK, fill_mode);

    max = 1;
    if((AI_SUCCESS == aiGetMaterialIntegerArray(mtl, AI_MATKEY_TWOSIDED, &two_sided, &max)) && two_sided)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);
}


// This function is taken from the examples coming with assimp
void OpenGLRenderInterface::recursiveRender(const struct aiScene *sc, const struct aiNode* nd) {
    unsigned int i;
    unsigned int n = 0, t;
    aiMatrix4x4 m = nd->mTransformation;

    // update transform
    aiTransposeMatrix4(&m);
    glPushMatrix();
    glMultMatrixf((float*)&m);

    // draw all meshes assigned to this node
    for (; n < nd->mNumMeshes; ++n) {
        const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

        glPushAttrib(GL_POLYGON_BIT | GL_LIGHTING_BIT);  // for applyMaterial()
        applyMaterial(sc->mMaterials[mesh->mMaterialIndex]);

        if(mesh->mNormals == NULL) {
            glDisable(GL_LIGHTING);
        } else {
            glEnable(GL_LIGHTING);
        }

        for (t = 0; t < mesh->mNumFaces; ++t) {
            const struct aiFace* face = &mesh->mFaces[t];
            GLenum face_mode;

            switch(face->mNumIndices) {
                case 1: face_mode = GL_POINTS; break;
                case 2: face_mode = GL_LINES; break;
                case 3: face_mode = GL_TRIANGLES; break;
                default: face_mode = GL_POLYGON; break;
            }

            glBegin(face_mode);

            for (i = 0; i < face->mNumIndices; i++) {
                int index = face->mIndices[i];
                if(mesh->mColors[0] != NULL)
                    glColor4fv((GLfloat*)&mesh->mColors[0][index]);
                if(mesh->mNormals != NULL)
                    glNormal3fv(&mesh->mNormals[index].x);
                glVertex3fv(&mesh->mVertices[index].x);
            }

            glEnd();
        }

        glPopAttrib();  // for applyMaterial()
    }

    // draw all children
    for (n = 0; n < nd->mNumChildren; ++n) {
        recursiveRender(sc, nd->mChildren[n]);
    }

    glPopMatrix();
}

void OpenGLRenderInterface::drawMesh(const Eigen::Vector3d& _scale, const aiScene* _mesh) {
    if(_mesh) {
        glPushMatrix();
        glScaled(_scale(0), _scale(1), _scale(2));
        recursiveRender(_mesh, _mesh->mRootNode);
        glPopMatrix();
    }
}

void OpenGLRenderInterface::drawList(GLuint index) {
    glCallList(index);
}

void OpenGLRenderInterface::compileList(dynamics::Skeleton* _skel) {
    if(_skel == 0)
        return;

    for (size_t i = 0; i < _skel->getNumBodyNodes(); i++) {
        compileList(_skel->getBodyNode(i));
    }
}

void OpenGLRenderInterface::compileList(dynamics::BodyNode* _node) {
    if(_node == 0)
        return;

    for (size_t i = 0; i < _node->getNumVisualizationShapes(); i++)
        compileList(_node->getVisualizationShape(i));
    for (size_t i = 0; i < _node->getNumCollisionShapes(); i++)
        compileList(_node->getCollisionShape(i));
}

//FIXME: Use polymorphism instead of switch statements
void OpenGLRenderInterface::compileList(dynamics::Shape* _shape) {
    if(_shape == 0)
        return;

    switch(_shape->getShapeType()) {
        case dynamics::Shape::BOX:
            break;
        case dynamics::Shape::CYLINDER:
            break;
        case dynamics::Shape::ELLIPSOID:
            break;
        case dynamics::Shape::PLANE:
            dterr << "PLANE is not supported yet." << std::endl;
            break;
        case dynamics::Shape::SOFT_MESH:
            // Do nothing
            break;
        case dynamics::Shape::MESH:
        {
            //FIXME: Separate these calls once BodyNode is refactored to contain
            // both a col Shape and vis Shape.
            dynamics::MeshShape* shapeMesh = dynamic_cast<dynamics::MeshShape*>(_shape);

            if(shapeMesh == 0)
                return;

            shapeMesh->setDisplayList(compileList(shapeMesh->getScale(), shapeMesh->getMesh()));

            break;
        }
        case dynamics::Shape::LINE_SEGMENT:
            // Do nothing
            break;
    }
}

GLuint OpenGLRenderInterface::compileList(const Eigen::Vector3d& _scale, const aiScene* _mesh) {
    if(!_mesh)
        return 0;

    // Generate one list
    GLuint index = glGenLists(1);
    // Compile list
    glNewList(index, GL_COMPILE);
    drawMesh(_scale, _mesh);
    glEndList();

    return index;
}

void OpenGLRenderInterface::draw(dynamics::Skeleton* _skel, bool _vizCol, bool _colMesh) {
    if(_skel == 0)
        return;

    for (size_t i = 0; i < _skel->getNumBodyNodes(); i++) {
        draw(_skel->getBodyNode(i), _vizCol, _colMesh);
    }
}

void OpenGLRenderInterface::draw(dynamics::BodyNode* _node, bool _vizCol, bool _colMesh) {
    if(_node == 0)
        return;

    // Get world transform
    Eigen::Isometry3d pose;
    pose = _node->getTransform();

    // GL calls
    if(_vizCol && _node->isColliding()) {
        glDisable(GL_TEXTURE_2D);
        glEnable(GL_COLOR_MATERIAL);
        glColor3f(1.0f, .1f, .1f);
    }

    glPushMatrix();
    glMultMatrixd(pose.data());

    if(_colMesh) {
        for (size_t i = 0; i < _node->getNumCollisionShapes(); i++)
            draw(_node->getCollisionShape(i));
    }
    else {
        for (size_t i = 0; i < _node->getNumVisualizationShapes(); i++)
            draw(_node->getVisualizationShape(i));
    }

    glColor3f(1.0f,1.0f,1.0f);
    glEnable( GL_TEXTURE_2D );
    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

//FIXME: Refactor this to use polymorphism.
void OpenGLRenderInterface::draw(dynamics::Shape* _shape) {
    if(_shape == 0)
        return;

    Eigen::Isometry3d pose = _shape->getLocalTransform();
    Eigen::Vector3d color = _shape->getColor();

    glPushMatrix();

    glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
    glEnable ( GL_COLOR_MATERIAL );

    glColor3d(color[0], color[1], color[2]);

    glMultMatrixd(pose.data());

    switch(_shape->getShapeType()) {
        case dynamics::Shape::BOX: {
            //FIXME: We are not in a glut instance
            dynamics::BoxShape* box = static_cast<dynamics::BoxShape*>(_shape);
            drawCube(box->getSize());
            break;
        }
        case dynamics::Shape::CYLINDER: {
            //FIXME: We are not in a glut instance
            dynamics::CylinderShape* cylinder = static_cast<dynamics::CylinderShape*>(_shape);
            drawCylinder(cylinder->getRadius(), cylinder->getHeight());
            break;
        }
        case dynamics::Shape::ELLIPSOID: {
            //FIXME: We are not in a glut instance
            dynamics::EllipsoidShape* ellipsoid = static_cast<dynamics::EllipsoidShape*>(_shape);
            drawEllipsoid(ellipsoid->getSize());
            break;
        }
        case dynamics::Shape::PLANE: {
            dterr << "PLANE shape is not supported yet." << std::endl;
            break;
        }
        case dynamics::Shape::MESH: {
            glDisable(GL_COLOR_MATERIAL); // Use mesh colors to draw

            dynamics::MeshShape* mesh = static_cast<dynamics::MeshShape*>(_shape);

            if(!mesh)
                break;
            else if(mesh->getDisplayList())
                drawList(mesh->getDisplayList());
            else
                drawMesh(mesh->getScale(), mesh->getMesh());

            break;
        }
        case dynamics::Shape::SOFT_MESH: {
            // Do nothing
            break;
        }
      case dynamics::Shape::LINE_SEGMENT: {
        dynamics::LineSegmentShape* lineSegments =
          static_cast<dynamics::LineSegmentShape*>(_shape);
        drawLineSegments(lineSegments->getVertices(),
                         lineSegments->getConnections());
      }
    }

    glDisable(GL_COLOR_MATERIAL);
    glPopMatrix();
}

void OpenGLRenderInterface::drawLineSegments(
    const std::vector<Eigen::Vector3d>& _vertices,
    const std::vector<Eigen::Vector2i>& _connections)
{
  glBegin(GL_LINES);
  for(const Eigen::Vector2i& c : _connections)
  {
    const Eigen::Vector3d& v1 = _vertices[c[0]];
    const Eigen::Vector3d& v2 = _vertices[c[1]];
    glVertex3f(v1[0], v1[1], v1[2]);
    glVertex3f(v2[0], v2[1], v2[2]);
  }
  glEnd();
}

void OpenGLRenderInterface::setPenColor(const Eigen::Vector4d& _col) {
    glColor4d(_col[0], _col[1], _col[2], _col[3]);
}

void OpenGLRenderInterface::setPenColor(const Eigen::Vector3d& _col) {
    glColor4d(_col[0], _col[1], _col[2], 1.0);
}

void OpenGLRenderInterface::setLineWidth(float _width) {
    glLineWidth(_width);
}

void OpenGLRenderInterface::readFrameBuffer(DecoBufferType _buffType, DecoColorChannel _ch, void* _pixels) {

}

void OpenGLRenderInterface::saveToImage(const char* _filename, DecoBufferType _buffType) {

}

} // namespace renderer
} // namespace dart
