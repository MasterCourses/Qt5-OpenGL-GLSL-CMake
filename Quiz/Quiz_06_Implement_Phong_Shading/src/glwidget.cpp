#include "glwidget.h"

const char* vertexShaderSource =
"attribute vec4 aPosition;                                                                                              \n"
"attribute vec4 aColor;                                                                                                 \n"
"attribute vec4 aNormal;                                                                                                \n"
"uniform mat4 uMVPMatrix;                                                                                               \n"
"uniform mat4 uModelMatrix;                                                                                             \n"
"uniform mat4 uNormalMatrix;                                                                                            \n" // correct normal matrix
"varying vec3 vNormal;                                                                                                  \n" // pass
"varying vec3 vPositionInWorld;                                                                                         \n" // pass
"varying vec4 vColor;                                                                                                   \n"
"void main(){                                                                                                           \n"
"   //TODO-1: transform 'aPosition' to clip space and store in 'gl_Position'                                            \n"
"   //TODO-2: transform 'aPosition' to world space and store its first three elements to 'vPositionInWorld'             \n"
"   //TODO-3: transform normal vector 'aNormal' to world space using 'uNormalMatrix' and store the result in 'vNormal', \n"
"   //        remember to renormalize the result before storing it to vNormal                                           \n"
"   //TODO-4: set 'aColor' to 'vColor'                                                                                  \n"
"}                                                                                                                      \n";

const char* fragmentShaderSource =
"uniform vec3 uLightPosition;                                                                                           \n" // for diffuse
"uniform vec3 uViewPosition;                                                                                            \n" // for specular
"uniform float uKa;                                                                                                     \n" // ambient reflection coefficient  (Ka)
"uniform float uKd;                                                                                                     \n" // diffuse reflection coefficient  (Kd)
"uniform float uKs;                                                                                                     \n" // specular reflection coefficient (Ks)
"uniform float uShininess;                                                                                              \n" // specular shininess
"varying vec3 vNormal;                                                                                                  \n" // pass
"varying vec3 vPositionInWorld;                                                                                         \n" // pass
"varying vec4 vColor;                                                                                                   \n"
"void main()                                                                                                            \n"
"{                                                                                                                      \n"
"    // let ambient and diffuse color are vColor                                                                        \n"
"    // (you can also input them from ouside and make it different)                                                     \n"
"    vec3 ambientLightColor = vColor.rgb;                                                                               \n" // Ia : ambient light intensity
"    vec3 diffuseLightColor = vColor.rgb;                                                                               \n" // Id : diffuse light intensity
"    // assume white specular light (you can also input it from ouside)                                                 \n"
"    vec3 specularLightColor = vec3(1.0, 1.0, 1.0);                                                                     \n" // Is : specular light intensity
"                                                                                                                       \n"
"    // TODO-5: calculate ambient light color using 'ambientLightColor' and 'uKa'                                       \n"
"                                                                                                                       \n"
"                                                                                                                       \n"
"                                                                                                                       \n" // normalize the v_Normal before using it,
"   vec3 normal = normalize(vNormal);                                                                                   \n" // before it comes from normal vectors interpolation
"                                                                                                                       \n"
"   // TODO-6: calculate diffuse light color using 'normal', 'uLightPosition'                                           \n"
"   //                                             ,'vPositionInWorld', 'diffuseLightColor', and 'uKd'                  \n"
"                                                                                                                       \n"
"    vec3 specular = vec3(0.0, 0.0, 0.0);                                                                               \n"
"    if(nDotL > 0.0) {                                                                                                  \n"
"       //TODO-7: calculate specular light color using 'normal', 'uLightPosition',                                      \n"
"       //                                             'vPositionInWorld',  'uViewPosition', 'uShininess',              \n"
"       //                                             'specularLightColor', and 'uKs'                                  \n"
"       //You probably can store the result of specular calculation into 'specular' variable                            \n"

"    }                                                                                                                  \n"
"                                                                                                                       \n"
"                                                                                                                       \n"
"   //TODO-8: sum up ambient, diffuse, specular light color from above calculation                                      \n"
"   //        and put them into 'gl_FragColor'                                                                          \n"
"	gl_FragColor = ???;                                                                                                 \n"
"}                                                                                                                      \n";

glWidget::glWidget(QWidget* parent)
    : QOpenGLWidget(parent)
{
    setMouseTracking(true);
}

glWidget::~glWidget()
{

}

void glWidget::initializeGL()
{
    // initialize env
    this->initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // set background color is black

    // Create and Compile : vertex/frag shader
    QOpenGLShader* vertexShader = new QOpenGLShader(QOpenGLShader::Vertex);
    vertexShader->compileSourceCode(vertexShaderSource);
    QOpenGLShader* fragmentShader = new QOpenGLShader(QOpenGLShader::Fragment);
    fragmentShader->compileSourceCode(fragmentShaderSource);

    // Create shader program
    program = new QOpenGLShaderProgram(this);
    // Attach shader to program
    program->addShader(vertexShader);
    program->addShader(fragmentShader);
    // link shader to program
    program->link();

}

void glWidget::paintGL()
{
    // Activate shader program
    program->bind();

    /////prepare attribute reference of the shader
    aPosition = glGetAttribLocation(program->programId(), "aPosition");
    aColor = glGetAttribLocation(program->programId(), "aColor");
    aNormal = glGetAttribLocation(program->programId(), "aNormal");

    uMVPMatrix = glGetUniformLocation(program->programId(), "uMVPMatrix"); // set the Uniform attribute for MVP Matrix 
    uModelMatrix = glGetUniformLocation(program->programId(), "uModelMatrix"); // set the Uniform attribute for Model Matrix 
    uNormalMatrix = glGetUniformLocation(program->programId(), "uNormalMatrix"); // set the Uniform attribute for Normal Matrix 
    uLightPosition = glGetUniformLocation(program->programId(), "uLightPosition"); // set the Uniform attribute for Light Position
    uViewPosition = glGetUniformLocation(program->programId(), "uViewPosition"); // set the Uniform attribute for View Position (for specular use)

    uKa = glGetUniformLocation(program->programId(), "uKa"); // set the Uniform attribute for ambient reflection coefficient Ka (0 <= Ka <= 1) 
    uKd = glGetUniformLocation(program->programId(), "uKd"); // set the Uniform attribute for diffuse reflection coefficient Kd (0 <= Kd <= 1) 
    uKs = glGetUniformLocation(program->programId(), "uKs"); // set the Uniform attribute for specular reflection coefficient Kd (0 <= Ks <= 1) 
    uShininess = glGetUniformLocation(program->programId(), "uShininess"); // set the Uniform attribute for specular shininess 


    /////create vertex buffer of triangle, color for later use
    initVertexBufferForLaterUse();

    glEnable(GL_DEPTH_TEST);

    modelMatrix = QMatrix4x4();
    mvpMatrix = QMatrix4x4();
    normalMatrix = QMatrix4x4();

    draw();

    // Release shader program
    program->release();

}

void glWidget::resizeGL(int w, int h)
{
    glViewport(-w / 2, w / 2, h / 2, -h / 2);
}

QSize glWidget::minimumSizeHint() const
{
    return QSize(100, 100);
}

QSize glWidget::sizeHint() const
{
    return QSize(800, 800);
}

void glWidget::mousePressEvent(QMouseEvent* event)
{

    setCursor(Qt::OpenHandCursor);
    QPoint clickPos = event->pos();
    float x = (float)clickPos.x();
    float y = (float)clickPos.y();
    QRect rect = this->geometry();
    if (rect.left() <= x && x < rect.right() && rect.top() <= y && y < rect.bottom()) {
        mouseLastX = x;
        mouseLastY = y;
        mouseDragging = true;
    }
}

void glWidget::mouseReleaseEvent(QMouseEvent* event)
{
    mouseDragging = false;
    setCursor(Qt::ArrowCursor);

}

void glWidget::mouseMoveEvent(QMouseEvent* event)
{
    QPoint clickPos = event->pos();

    float x = (float)clickPos.x();
    float y = (float)clickPos.y();
    if (mouseDragging) {
        setCursor(Qt::ClosedHandCursor);
        float dx = (x - mouseLastX) / 10.0 * M_PI;
        float dy = (y - mouseLastY) / 10.0 * M_PI;

        angleX += dx; //yes, x for y, y for x, this is right
        angleY += dy;
    }
    mouseLastX = x;
    mouseLastY = y;

    draw();

}

void glWidget::initVertexBufferForLaterUse()
{
    // each face of a cube is divided into 2 triangles
    numVertices = (3 * 6) * 6 / 3; // 6 faces (each face has 2 triagnles) (each triangle has 3 vertices)

    GLfloat  vertices[] = { // cube
            1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, 1.0, //front
            1.0, 1.0, 1.0, 1.0, -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, 1.0, -1.0, //right
            1.0, 1.0, 1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, //up
            -1.0, 1.0, 1.0, -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0, 1.0, 1.0, //left
            -1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, -1.0,  1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, 1.0, //bottom
            1.0, -1.0, -1.0, -1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, -1.0, -1.0, -1.0, 1.0, -1.0, 1.0, 1.0, -1.0 //back
    };

    GLfloat colors[] = {   //cube's color
            0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, //front
            0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, //right
            1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, 1.0, 0.4, 0.4, //up
            1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, //left
            1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, //bottom
            0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, 0.4, 1.0, 1.0, //back
    };

    GLfloat normals[] = {   //cube's normal
        0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, //front
        1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, //right
        0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, 0.0, 1.0, 0.0, //up
        -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, //left
        0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, //bottom
        0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0, 0.0, 0.0, -1.0 //back
    };

    // initArrayBuffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    initAttributeVariable(aPosition, 3, vertexBuffer);
    glGenBuffers(1, &colorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    initAttributeVariable(aColor, 3, colorBuffer);
    glGenBuffers(1, &normalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    initAttributeVariable(aNormal, 3, normalBuffer);

    glBindBuffer(GL_ARRAY_BUFFER, NULL);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

}

void glWidget::initAttributeVariable(GLuint arrtibute, GLint num, GLuint buffer)
{
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glVertexAttribPointer(arrtibute, num, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(arrtibute);
}


void glWidget::draw()
{
    //model Matrix (part of the mvp matrix)
    modelMatrix.setToIdentity();
    modelMatrix.rotate(-angleY, 1, 0, 0); //for mouse rotation
    modelMatrix.rotate(angleX, 0, 1, 0); //for mouse rotation
    modelMatrix.translate(0.0, 0.0, -1.0);
    modelMatrix.scale(1.0, 0.5, 2.0);
    //mvp: projection * view * model matrix  
    mvpMatrix.setToIdentity();
    mvpMatrix.perspective(30, 1, 1, 100); // verticalAngel, aspectRatio, nearPlane, farPlane
    mvpMatrix.lookAt(QVector3D(cameraX, cameraY, cameraZ), QVector3D(0, 0, 0), QVector3D(0, 1, 0)); // lookAt(eye, center, up)
    mvpMatrix *= modelMatrix;

    // normal matrix
    normalMatrix = modelMatrix.inverted();
    normalMatrix.transposed();

    glUniform3f(uLightPosition, 0.0, 0.0, 3.0);
    glUniform3f(uViewPosition, cameraX, cameraY, cameraZ);

    glUniform1f(uKa, 0.2f);
    glUniform1f(uKd, 0.7f);
    glUniform1f(uKs, 1.0f);
    glUniform1f(uShininess, 10.0f);

    glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, mvpMatrix.data()); //pass current model-view-projection Matrix to shader
    glUniformMatrix4fv(uModelMatrix, 1, GL_FALSE, modelMatrix.data()); //pass current model-view-projection Matrix to shader
    glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, normalMatrix.data()); //pass current model-view-projection Matrix to shader

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, numVertices);
    update();
}
