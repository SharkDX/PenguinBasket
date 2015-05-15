#include "Profiler.h"


int Profiler::DRAW_CALLS = 0;
int Profiler::FPS = 0;
double* Profiler::Time = 0;
bool Profiler::Pause = false;
double Profiler::DrawTime = 0;
bool Profiler::Multiplayer = false;
int Profiler::Ping = 0;
unsigned long Profiler::RamUsage = 0;
unsigned long Profiler::CpuUsage = 0;