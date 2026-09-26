#include <iostream>
#include <string>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Порт, на котором будет работать наш сайт (например, http://localhost:8080)
const int PORT = 8080;

// Тот самый HTML-код из Части 1 (перенесенный в C++ строку)
const std::string HTML_RESPONSE = 
    "HTTP/1.1 200 OK\r\n"
    "Content-Type: text/html; charset=UTF-8\r\n"
    "Connection: close\r\n\r\n"
    "<!DOCTYPE html>\n"
    "<html lang=\"ru\">\n"
    "<head>\n"
    "    <meta charset=\"UTF-8\">\n"
    "    <meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">\n"
    "    <title>Расписание | Хайыракан 7 класс</title>\n"
    "    <style>\n"
    "        /* Вставляем сюда наш CSS из 2 части, чтобы все было в одном файле для супер-скорости */\n"
    "        * { margin: 0; padding: 0; box-sizing: border-box; font-family: sans-serif; }\n"
    "        body { background-color: #0b0f19; color: #e2e8f0; min-height: 100vh; display: flex; flex-direction: column; }\n"
    "        .container { width: 100%; max-width: 1200px; margin: 0 auto; padding: 0 20px; }\n"
    "        .main-header { background: linear-gradient(135deg, #1e1b4b 0%, #0f172a 100%); padding: 40px 0; text-align: center; border-bottom: 2px solid #3b82f6; }\n"
    "        .school-badge { background-color: #3b82f6; color: white; font-size: 0.8rem; font-weight: bold; padding: 4px 12px; border-radius: 20px; margin-bottom: 10px; display: inline-block; }\n"
    "        h1 { font-size: 2.8rem; text-transform: uppercase; color: white; }\n"
    "        .subtitle { color: #94a3b8; }\n"
    "        main { flex: 1; padding: 40px 0; }\n"
    "        .schedule-grid { display: grid; grid-template-columns: repeat(auto-fit, minmax(300px, 1fr)); gap: 25px; }\n"
    "        .day-card { background-color: #111827; border: 1px solid #1f2937; border-radius: 16px; padding: 24px; transition: all 0.3s ease; }\n"
    "        .day-card:hover { transform: translateY(-5px); border-color: #10b981; }\n"
    "        .day-title { font-size: 1.4rem; color: white; margin-bottom: 20px; padding-bottom: 10px; border-bottom: 1px solid #374151; }\n"
    "        .lesson-list { list-style: none; }\n"
    "        .lesson-list li { display: flex; align-items: center; padding: 12px 0; border-bottom: 1px dashed #1f2937; }\n"
    "        .lesson-num { background-color: #1f2937; color: #10b981; font-weight: bold; width: 28px; height: 28px; border-radius: 8px; display: flex; align-items: center; justify-content: center; margin-right: 15px; }\n"
    "        .main-footer { background-color: #090d16; padding: 25px 0; text-align: center; border-top: 1px solid #1f2937; font-size: 0.9rem; color: #64748b; }\n"
    "        strong { color: #3b82f6; }\n"
    "        #ping { color: #10b981; font-weight: bold; background-color: rgba(16, 185, 129, 0.1); padding: 2px 6px; border-radius: 4px; }\n"
    "    </style>\n"
    "</head>\n"
    "<body>\n"
    "    <header class=\"main-header\">\n"
    "        <div class=\"container\">\n"
    "            <span class=\"school-badge\">7 КЛАСС</span>\n"
    "            <h1>Хайыракан</h1>\n"
    "            <p class=\"subtitle\">Умное и быстрое расписание уроков</p>\n"
    "        </div>\n"
    "    </header>\n"
    "    <main class=\"container\">\n"
    "        <div class=\"schedule-grid\">\n"
    "            <section class=\"day-card\">\n"
    "                <h2 class=\"day-title\">Понедельник</h2>\n"
    "                <ul class=\"lesson-list\">\n"
    "                    <li><span class=\"lesson-num\">1</span> Физика</li>\n"
    "                    <li><span class=\"lesson-num\">2</span> Алгебра</li>\n"
    "                    <li><span class=\"lesson-num">3</span> Русский язык</li>\n"
    "                </ul>\n"
    "            </section>\n"
    "            <section class=\"day-card\">\n"
    "                <h2 class=\"day-title\">Вторник</h2>\n"
    "                <ul class=\"lesson-list\">\n"
    "                    <li><span class=\"lesson-num">1</span> Геометрия</li>\n"
    "                    <li><span class=\"lesson-num">2</span> Литература</li>\n"
    "                    <li><span class=\"lesson-num">3</span> Английский язык</li>\n"
    "                </ul>\n"
    "            </section>\n"
    "        </div>\n"
    "    </main>\n"
    "    <footer class=\"main-footer\">\n"
    "        <div class=\"container\">\n"
    "            <p>Сервер работает на сверхбыстром движке <strong>C++</strong></p>\n"
    "            <p>Время ответа сервера: <span id=\"ping\">~0.4 ms</span></p>\n"
    "        </div>\n"
    "    </footer>\n"
    "</body>\n"
    "</html>";

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // 1. Создаем сокет (инструмент для работы с сетью)
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        std::cerr << "Ошибка создания сокета!" << std::endl;
        return 1;
    }

    // Привязываем сокет к порту, чтобы система не ругалась при перезапуске
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY; // Слушать все входящие адреса
    address.sin_family = AF_INET;
    address.sin_port = htons(PORT);

    // 2. Привязываем адрес и порт к сокету
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        std::cerr << "Ошибка bind (возможно, порт занят)!" << std::endl;
        return 1;
    }

    // 3. Включаем режим «прослушивания» сети
    if (listen(server_fd, 10) < 0) {
        std::cerr << "Ошибка listen!" << std::endl;
        return 1;
    }

    std::cout << ">>> Сервер запущен на http://localhost:" << PORT << std::endl;
    std::cout << ">>> Нажми Ctrl+C для остановки." << std::endl;

    // 4. Бесконечный цикл сервера: ждем людей и отдаем расписание
    while (true) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_list_type*)&addrlen);
        if (new_socket < 0) {
            std::cerr << "Ошибка приема соединения!" << std::endl;
            continue;
        }

        // Читаем, что запросил браузер (в данном случае нам неважно, мы всегда отдаем наш сайт)
        char buffer[1024] = {0};
        read(new_socket, buffer, 1024); 

        // Мгновенно отправляем HTML-страницу со встроенным CSS дизайном
        send(new_socket, HTML_RESPONSE.c_str(), HTML_RESPONSE.length(), 0);
        
        // Закрываем соединение, чтобы браузер понял, что загрузка окончена
        close(new_socket);
    }

    close(server_fd);
    return 0;
  }
