#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <format>
#include <string>
#include <unordered_map>  
#include <fstream>
#include <string>
#include <print>
#include <iostream>


int main() {
    std::unordered_map<std::string, std::pair<int, bool>> values;

    std::ifstream file("x-axis.csv");
    if (file.bad()) {
        std::println(std::cerr, "ERROR: Could not open 'x-axis.csv'");
        return 1;
    }

    do {
        std::string x;
        std::getline(file, x, ',');

        int idx = x.find('\n');

        if (idx != -1) {
            x.replace(idx, 1, "");
        }

        values.insert({x, {0, false}});
    } while (!file.eof());
    file.close();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(144);

    InitWindow(1280, 720, "Spread Maker");

    rlImGuiSetup(true);

    bool open = true;

    char outputBuffer[50] = "Output.png";

    const int size = 35;

    while (!WindowShouldClose()) {
        bool shouldTakeScreenshot = false;
        BeginDrawing();

        ClearBackground(GRAY);
        DrawLine(150, 30, 150, 70 + values.size() * 100, WHITE);

        int i = 0;
        for (const auto &pair : values) {
            DrawText(std::format("{}", pair.first).c_str(), 50, 50 + i * 100, size, WHITE);
            for (int j = 0; j < pair.second.first; ++j) {
                Color color = BLUE;
                if (j == 0 && pair.second.second) {
                    color = ORANGE;
                }
                DrawCircle(200 + j * 100, 50 + i * 100, size, color);
            }
            ++i;
        }

        rlImGuiBegin();

        if (ImGui::Begin("Config", &open)) {
            for (auto &pair : values) {
                ImGui::SliderInt(pair.first.c_str(), &pair.second.first, 0, 4);
                ImGui::PushID(pair.first.c_str());
                ImGui::Checkbox("Myself", &pair.second.second);
                ImGui::PopID();
            }

            ImGui::InputText("Output", outputBuffer, 50);

            if (ImGui::Button("Generate")) {
                shouldTakeScreenshot = true;
            }
        }
        ImGui::End();


        rlImGuiEnd();

        EndDrawing();

        if (shouldTakeScreenshot) {
            TakeScreenshot(outputBuffer);
        }
    }

    rlImGuiShutdown();

    CloseWindow();
}
