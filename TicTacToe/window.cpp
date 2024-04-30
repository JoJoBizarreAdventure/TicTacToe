#include "window.h"

#include <iostream>

#include "humanPlayer.h"
#include "simpleAIPlayer.h"
#include "minMaxAIPlayer.h"

#include "fmt/core.h"

Window::Window(int w, int h, bool p1AI, bool p2AI, bool simpleAI) {
#pragma region init
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

#pragma endregion

#pragma region create window
    int monitorCount;
    GLFWmonitor **pMonitors = glfwGetMonitors(&monitorCount);
    if (monitorCount <= 0) {
        std::cout << "No monitor found" << std::endl;
        glfwTerminate();
        return;
    }
    const GLFWvidmode *mode = glfwGetVideoMode(pMonitors[0]);
    int monitorWidth = mode->width, monitorHeight = mode->height;

    height = h > 0 ? h : static_cast<int>(monitorHeight * 0.9);
    width = w > 0 ? w : height + (height >> 1);

    window = glfwCreateWindow(width, height, "TicTacToe\tfps: 0.0", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glfwSetWindowPos(window, (monitorWidth - width) >> 1, (monitorHeight - height) >> 1);
    glfwSetWindowAspectRatio(window, width, height);
#pragma endregion

    if (!gladLoadGL((GLADloadfunc) glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return;
    }

    ui = new Ui(width, height);
    ui->linkGridStates(chessboardState);
    dataSource = ui->rawData();

#pragma region create gl screen image
    // texture object
    glGenTextures(1, &screenColor);
    glBindTexture(GL_TEXTURE_2D, screenColor);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    // vertex info
    float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
            // positions   // texCoords
            -1.0f, 1.0f, 0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
            1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f, 1.0f, 0.0f, 1.0f,
            1.0f, -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f
    };

    // screen quad VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));

    // screen shader
    int success;
    char infoLog[512];

    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    const char *vertexSource = "#version 330 core\n"
                               "layout (location = 0) in vec2 aPos;\n"
                               "layout (location = 1) in vec2 aTexCoords;\n"
                               "out vec2 TexCoords;\n"
                               "void main()\n"
                               "{\n"
                               "    TexCoords = vec2(aTexCoords.x, 1.0 - aTexCoords.y);\n"
                               "    gl_Position = vec4(aPos, 1.0, 1.0);\n"
                               "}";
    glShaderSource(vertexShader, 1, &vertexSource, nullptr);
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    const char *fragSource = "#version 330 core\n"
                             "out vec4 FragColor;\n"
                             "in vec2 TexCoords;\n"
                             "uniform sampler2D renderResult;\n"
                             "void main()\n"
                             "{\n"
                             "    vec4 tex = texture(renderResult, TexCoords);\n"
                             "    FragColor = tex.zyxw;\n"
                             "}";
    glShaderSource(fragmentShader, 1, &fragSource, nullptr);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    shader = glCreateProgram();
    glAttachShader(shader, vertexShader);
    glAttachShader(shader, fragmentShader);
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINK_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUniform1i(glGetUniformLocation(shader, "renderResult"), 0);
#pragma endregion

    if (p1AI) {
        if (simpleAI)
            player1 = new SimpleAIPlayer(PLAYER_ONE_SYMBOL, chessboardState);
        else
            player1 = new MinMaxAIPlayer(PLAYER_ONE_SYMBOL, chessboardState);
    } else {
        player1 = new HumanPlayer(PLAYER_ONE_SYMBOL, window);
    }

    if (p2AI) {
        if (simpleAI)
            player2 = new SimpleAIPlayer(PLAYER_TWO_SYMBOL, chessboardState);
        else
            player2 = new MinMaxAIPlayer(PLAYER_TWO_SYMBOL, chessboardState);
    } else {
        player2 = new HumanPlayer(PLAYER_TWO_SYMBOL, window);
    }

    ui->bindPlayers(player1, player2);

    currentPlayer = player1;

    ResetChessboard();
}

Window::~Window() {
    glDeleteProgram(shader);
    glDeleteBuffers(1, &quadVBO);
    glDeleteVertexArrays(1, &quadVAO);

    glfwTerminate();

    delete ui;
    delete player1;
    delete player2;
}

bool Window::changeState(GridState &state, const GridState target) {
    if (state > 0)
        return false;
    state = target;
    return true;
}

void Window::ResetChessboard() {
    for (auto &row: chessboardState) {
        for (auto &grid: row) {
            grid = Empty;
        }
    }
    stepCnt = 0;
    ui->refreshUI();
    inputEnable = true;

    if (whoIsFirst)
        currentPlayer = player1;
    else
        currentPlayer = player2;
}

void Window::mouseEventHandle(bool click, std::pair<int, int> index) {
    if (lastPointTo != index && lastPointTo != POINT_TO_NOTHING) {
        changeState(chessboardState[lastPointTo.first][lastPointTo.second], Empty);
    }

    if (index == POINT_TO_NOTHING) {
        lastPointTo = POINT_TO_NOTHING;
        return;
    }

    int symbol = currentPlayer->GetSymbol();

    if (click) {
        bool inputTakeEffect = changeState(chessboardState[index.first][index.second],
                                           static_cast<GridState>(std::abs(symbol)));
        if (inputTakeEffect) {
            stepCnt++;
            bool isWin = checkIfWin(index.first, index.second);
            if (isWin) {
                currentPlayer->addScore();
                whoIsFirst = currentPlayer == player2;
                inputEnable = false;
            }
            bool isDraw = checkIfDraw();
            if (isDraw) {
                ResetChessboard();
            }
            shiftPlayer();
        }

        lastPointTo = POINT_TO_NOTHING;
    } else if (lastPointTo != index) {
        changeState(chessboardState[index.first][index.second], static_cast<GridState>(symbol));
        lastPointTo = index;
    }
}


void Window::keyboardEventHandle() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        ResetChessboard();
    } else if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
        player1->clearScore();
        player2->clearScore();
    } else if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS && !inputEnable) {
        ResetChessboard();
    }
}

void Window::applyData() {
    glUseProgram(shader);

    glBindVertexArray(quadVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, screenColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, dataSource);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

void Window::loop() {
    double lastTime = glfwGetTime();
    int nFrames = 0;
    while (!glfwWindowShouldClose(window)) {
        if (inputEnable) {
            auto event = currentPlayer->getStep();
            if (currentPlayer->isHuman()) {
                event.index = ui->mousePosition2Index(event.index.first, event.index.second);
            }
            mouseEventHandle(event.clicked, event.index);
        }
        keyboardEventHandle();

        ui->renderUI();
        applyData();

        double delta = glfwGetTime() - lastTime;
        nFrames++;
        if (delta >= 1.0) {
            double fps = nFrames / delta;

            glfwSetWindowTitle(window, fmt::format("TicTacToe\tfps: {:.4}", fps).c_str());
            nFrames = 0;
            lastTime = glfwGetTime();
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::shiftPlayer() {
    currentPlayer = currentPlayer == player1 ? player2 : player1;
}

bool Window::checkIfWin(int y, int x) {
    // check x axis
    if (chessboardState[y][0] == chessboardState[y][1] && chessboardState[y][1] == chessboardState[y][2]) {
        ui->setEndAnim(0, y, 2, y);
        return true;
    }
    // check y axis
    if (chessboardState[0][x] == chessboardState[1][x] && chessboardState[1][x] == chessboardState[2][x]) {
        ui->setEndAnim(x, 0, x, 2);
        return true;
    }
    // check ortho if valid
    if (x - y == 0 && chessboardState[0][0] == chessboardState[1][1] &&
        chessboardState[1][1] == chessboardState[2][2]) {
        ui->setEndAnim(0, 0, 2, 2);
        return true;
    }
    if (x + y == 2 && chessboardState[0][2] == chessboardState[1][1] &&
        chessboardState[1][1] == chessboardState[2][0]) {
        ui->setEndAnim(0, 2, 2, 0);
        return true;
    }

    return false;
}

bool Window::checkIfDraw() const {
    return stepCnt == 9;
}






