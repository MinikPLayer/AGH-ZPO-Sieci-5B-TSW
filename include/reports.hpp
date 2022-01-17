// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once
#include "factory.hpp"
#include <set>

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

class SpecificTurnsReportNotifier
{
    std::set<Time> timeOffsets;

public:
    SpecificTurnsReportNotifier(std::set<Time> t): timeOffsets(t) {}
    bool should_generate_report(Time t)
    {
        return timeOffsets.find(t) != timeOffsets.end();
    }
};

class IntervalReportNotifier
{
    TimeOffset interval;
public:
    IntervalReportNotifier(TimeOffset to): interval(to) {}
    bool should_generate_report(Time t)
    {
        return t % interval == 0;
    }
};