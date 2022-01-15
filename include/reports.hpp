// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#pragma once
#include "factory.hpp"

void generate_structure_report(const Factory& f, std::ostream& os);
void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t);

class SpecificTurnsReportNotifier
{
    SpecificTurnsReportNotifier(std::set<Time>);
    bool should_generate_report(Time t);
};

class IntervalReportNotifier
{
    IntervalReportNotifier(TimeOffset to);
    bool should_generate_report(Time t);
};