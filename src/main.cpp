#include <raylib.h>
#include <rlImGui.h>
#include <imgui.h>
#include <point.h>

#include <format>
#include <string>
#include <fstream>
#include <string>
#include <print>
#include <iostream>
#include <vector>


int main() {
    std::vector<Point> values;

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

        values.push_back(Point{x, 0});
    } while (!file.eof());
    file.close();

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    SetTargetFPS(144);

    InitWindow(1280, 720, "Spread Maker");

    rlImGuiSetup(true);

    bool open = true;
    char outputBuffer[999] = "Output.png";
    const int size = 20;
    int myselfIdx = 0;

    while (!WindowShouldClose()) {
        bool shouldTakeScreenshot = false;
        BeginDrawing();

        ClearBackground(GRAY);
        DrawLine(30, 600, 50 + values.size() * 100, 600, WHITE);

        for (int i = 0; i < values.size(); ++i) {
            DrawText(values[i].label.c_str(), 40 + i * 100, 650, size, WHITE);
            for (int j = 0; j < values[i].count; ++j) {
                Color color = BLUE;
                if (j == 0 && i == myselfIdx) {
                    color = ORANGE;
                }
                DrawCircle(40 + i * 100, 550 - j * 100, size, color);
            }
        }

        rlImGuiBegin();

        if (ImGui::Begin("Config", &open)) {
            for (int i = 0; i < values.size(); ++i) {
                ImGui::SliderInt(values[i].label.c_str(), &values[i].count, 0, 4);
                if (values[i].count > 0) {
                    ImGui::PushID(values[i].label.c_str());
                    ImGui::RadioButton("Myself", &myselfIdx, i);
                    ImGui::PopID();
                }
            }

            ImGui::InputText("Output", outputBuffer, 999);

            if (ImGui::Button("Generate")) {
                shouldTakeScreenshot = true;
            }
        }
        ImGui::End();


        rlImGuiEnd();

        EndDrawing();

        if (shouldTakeScreenshot) {
            auto img = LoadImageFromScreen();
            auto bounds = Rectangle{0,0, float(60 + values.size() * 100), 675};
            ImageCrop(&img, bounds);

            ExportImage(img, outputBuffer);

            shouldTakeScreenshot = false;
        }
    }

    rlImGuiShutdown();

    CloseWindow();
}
