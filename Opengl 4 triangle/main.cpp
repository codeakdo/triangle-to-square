#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <cmath>


const int WIDTH = 800;
const int HEIGHT = 600;

// Üçgen durumu
int currentTriangleState = 0;
std::vector<float> triangleVertices;

// Direction tracking (-1 for none, 0 for clockwise, 1 for counter-clockwise)
int currentDirection = -1;

// Vertex Shader
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

// Fragment Shader
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

// Shader program oluşturma
unsigned int createShaderProgram() {
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// İlk üçgeni oluşturma (Y eksenine paralel, sağa bakan)
void createInitialTriangle() {
    triangleVertices = {
        0.0f, 0.0f, 0.0f,    // İlk köşe (merkez)
        -0.5f, 0.5f, 0.0f,    // Sağ üst köşe
        -0.5f, -0.5f, 0.0f    // Sol alt köşe
    };
    currentTriangleState = 1;
}

// Sıradaki üçgeni ekle (saat yönünde)
void addNextTriangle() {
    switch(currentTriangleState) {
        case 1: // X eksenine paralel, aşağıya bakan üçgen
            triangleVertices.insert(triangleVertices.end(), {
                0.0f, 0.0f, 0.0f,     // İlk köşe
                -0.5f, 0.5f, 0.0f,    // Sol alt köşe
                0.5f, 0.5f, 0.0f     // Sağ alt köşe
            });
            currentTriangleState = 2;
            break;
        case 2: // Y eksenine paralel, sola bakan üçgen
            triangleVertices.insert(triangleVertices.end(), {
                0.0f, 0.0f, 0.0f,     // İlk köşe
                0.5f, 0.5f, 0.0f,    // Sol alt köşe
                0.5f, -0.5f, 0.0f      // Sağ üst köşe
            });
            currentTriangleState = 3;
            break;
        case 3: // X eksenine paralel, yukarıya bakan üçgen
            triangleVertices.insert(triangleVertices.end(), {
                0.0f, 0.0f, 0.0f,     // İlk köşe
                -0.5f, -0.5f, 0.0f,     // Sol üst köşe
                0.5f, -0.5f, 0.0f      // Sağ üst köşe
            });
            currentTriangleState = 4;
            break;
    }
}

// Üçgenleri saat yönünün tersi yönünde ekle
void addCounterClockwiseTriangle() {
    switch(currentTriangleState) {
        case 1: // X eksenine paralel, yukarıya bakan üçgen (normalde case 3)
            triangleVertices.insert(triangleVertices.end(), {
                0.0f, 0.0f, 0.0f,      // İlk köşe
                -0.5f, -0.5f, 0.0f,    // Sol üst köşe
                0.5f, -0.5f, 0.0f      // Sağ üst köşe
            });
            currentTriangleState = 2;
            break;
        case 2: // Y eksenine paralel, sola bakan üçgen (normalde case 2)
            triangleVertices.insert(triangleVertices.end(), {
                0.0f, 0.0f, 0.0f,     // İlk köşe
                0.5f, -0.5f, 0.0f,    // Sağ alt köşe
                0.5f, 0.5f, 0.0f      // Sağ üst köşe
            });
            currentTriangleState = 3;
            break;
        case 3: // X eksenine paralel, aşağıya bakan üçgen (normalde case 1)
            triangleVertices.insert(triangleVertices.end(), {
                0.0f, 0.0f, 0.0f,     // İlk köşe
                0.5f, 0.5f, 0.0f,     // Sağ alt köşe
                -0.5f, 0.5f, 0.0f     // Sol alt köşe
            });
            currentTriangleState = 4;
            break;
    }
}

// Üçgeni sil ve bir önceki duruma geri dön
void removeLastTriangle() {
    if (currentTriangleState > 0) {
        // Bir üçgeni silmek için son 9 vertex'i kaldır (3 nokta × 3 koordinat)
        if (triangleVertices.size() >= 9) {
            triangleVertices.resize(triangleVertices.size() - 9);
        } else {
            // İlk üçgeni siliyorsa, tüm vertexleri temizle
            triangleVertices.clear();
        }
        currentTriangleState--;
    }
}

// Mouse click callback fonksiyonu
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        // Kullanıcının hangi yönde ekleme/silme yapmak istediğini belirle
        int requestedDirection = (button == GLFW_MOUSE_BUTTON_LEFT) ? 0 : 1;
        
        // Hiç üçgen yoksa: Başlangıç üçgenini oluştur
        if (currentTriangleState == 0) {
            createInitialTriangle();
            currentDirection = requestedDirection;
            return;
        }
        
        // Eğer aynı yöndeki tıklamaya devam ediyorsa, üçgen ekle
        if (requestedDirection == currentDirection) {
            if (currentTriangleState < 4) {
                if (button == GLFW_MOUSE_BUTTON_LEFT) {
                    addNextTriangle();
                } else {
                    addCounterClockwiseTriangle();
                }
            }
        }
        // Eğer kullanıcı tıklama yönünü değiştirmişse, üçgenleri sil
        else {
            // İlk üçgen dahil, tüm üçgenleri sil
            removeLastTriangle();
            
            // Eğer tüm üçgenler silindiyse, yönü değiştir ve yeni üçgen oluştur
            if (currentTriangleState == 0) {
                currentDirection = requestedDirection;
                createInitialTriangle();
            }
        }
    }
}

int main() {
    // GLFW başlatma
    if (!glfwInit()) {
        std::cerr << "GLFW başlatılamadı!" << std::endl;
        return -1;
    }

    // OpenGL versiyonu ve profil ayarları
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Pencere oluşturma
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Üçgenden Kare Oluşturucu", NULL, NULL);
    if (!window) {
        std::cerr << "Pencere oluşturulamadı!" << std::endl;
        glfwTerminate();
        return -1;
    }

    // OpenGL context'ini ayarlama
    glfwMakeContextCurrent(window);

    // GLEW başlatma
    if (glewInit() != GLEW_OK) {
        std::cerr << "GLEW başlatılamadı!" << std::endl;
        return -1;
    }

    // Mouse click callback'i
    glfwSetMouseButtonCallback(window, mouseButtonCallback);

    // Shader program oluşturma
    unsigned int shaderProgram = createShaderProgram();

    // VAO ve VBO oluşturma
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Ana döngü
    while (!glfwWindowShouldClose(window)) {
        // Ekranı temizleme
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Vertex verilerini güncelleme
        if (!triangleVertices.empty()) {
            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, triangleVertices.size() * sizeof(float),
                         triangleVertices.data(), GL_STATIC_DRAW);

            // Vertex özellikleri ayarlama
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            // Shader programını kullanma
            glUseProgram(shaderProgram);

            // Üçgenleri çizme
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); 
            //border line
            glDrawArrays(GL_TRIANGLES, 0, triangleVertices.size() / 3);
        }

        // Arabellek değişimi
        glfwSwapBuffers(window);

        // Olayları işleme
        glfwPollEvents();
    }

    // Temizlik
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // GLFW sonlandırma
    glfwTerminate();
    return 0;
}
