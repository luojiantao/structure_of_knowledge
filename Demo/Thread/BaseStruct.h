#pragma once

namespace ThreadPoolDemo{
enum Type{
   Io = 0,
   Timing,
   Calculate
};

enum Priority{
    Normal = 0,
    Advanced
};
struct TaskAttr{
	TaskAttr(Type t = Type::Io, Priority pri = Priority::Normal, int tim = 0, bool one = true):
	type(t), priority(pri), time(tim), once(one){}
    Type type = Type::Io;
    Priority priority = Priority::Normal;
    int time = 0;
	bool once = true;
};
}
