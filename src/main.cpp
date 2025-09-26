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

    SetConfigFlags(FLAG_WINDOW_RESIZABLE | FLAG_VSYNC_HINT);
    SetTargetFPS(144);

    InitWindow(1280, 720, "Spread Maker");

    rlImGuiSetup(true);

    bool open = true;
    char outputBuffer[999] = "Output.png";
    const int size = 20;
    int myselfIdx = 0;

    float selfColor[] = {float(ORANGE.r) / 255, float(ORANGE.g) / 255, float(ORANGE.b) / 255};
    float teamColor[] = {0, 0, 1};
    float backgroundColor[] = {float(GRAY.r) / 255, float(GRAY.g) / 255, float(GRAY.b) / 255};
    float lineColor[] = {1, 1, 1};
    float fontColor[] = {1, 1, 1};

    while (!WindowShouldClose()) {
        bool shouldTakeScreenshot = false;
        BeginDrawing();

        ClearBackground(ColorFromNormalized(Vector4{backgroundColor[0], backgroundColor[1], backgroundColor[2], 1}));
        DrawLine(30, 600, 50 + values.size() * 100, 600, ColorFromNormalized(Vector4{lineColor[0], lineColor[1], lineColor[2], 1}));

        for (int i = 0; i < values.size(); ++i) {
            DrawText(values[i].label.c_str(), 40 + i * 100, 650, size, ColorFromNormalized(Vector4{fontColor[0], fontColor[1], fontColor[2], 1}));
            for (int j = 0; j < values[i].count; ++j) {
                Color color = ColorFromNormalized(Vector4{teamColor[0], teamColor[1], teamColor[2], 1});
                if (j == 0 && i == myselfIdx) {
                    color = ColorFromNormalized(Vector4{selfColor[0], selfColor[1], selfColor[2], 1});
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


            ImGui::ColorEdit3("BackgroundColor", backgroundColor);
            ImGui::ColorEdit3("Divider Color", lineColor);
            ImGui::ColorEdit3("Font Color", fontColor);
            ImGui::ColorEdit3("Myself Color", selfColor);
            ImGui::ColorEdit3("Team Color", teamColor);

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
