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

// TODO: Better Name, Seperate File
Color normToReg(float *col) {
    return ColorFromNormalized(Vector4{col[0], col[1], col[2], 1});
}

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
        BeginDrawing();

        ClearBackground(normToReg(backgroundColor));
        DrawLine(30, 600, 50 + values.size() * 100, 600, normToReg(lineColor));

        for (int i = 0; i < values.size(); ++i) {
            DrawText(values[i].label.c_str(), 40 + i * 100, 650, size, normToReg(fontColor));
            for (int j = 0; j < values[i].count; ++j) {
                Color color = normToReg(teamColor);
                if (j == 0 && i == myselfIdx) {
                    color = normToReg(selfColor);
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
                auto img = GenImageColor(float(60 + values.size() * 100), 675, normToReg(backgroundColor));

                ImageDrawLine(&img, 30, 600, 50 + values.size() * 100, 600, normToReg(lineColor));

                for (int i = 0; i < values.size(); ++i) {
                    ImageDrawText(&img, values[i].label.c_str(), 40 + i * 100, 650, size, normToReg(fontColor));
                    for (int j = 0; j < values[i].count; ++j) {
                        Color color = normToReg(teamColor);
                        if (j == 0 && i == myselfIdx) {
                            color = normToReg(selfColor);
                        }
                        ImageDrawCircle(&img, 40 + i * 100, 550 - j * 100, size, color);
                    }
                }

                ExportImage(img, outputBuffer);
            }
        }
        ImGui::End();


        rlImGuiEnd();

        EndDrawing();
    }

    rlImGuiShutdown();

    CloseWindow();
}
