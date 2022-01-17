// 5B: Tomecki (408146), Sztefko (407388), Walawski (406822)
#include "reports.hpp"
#include "factory.hpp"

void generate_structure_report(const Factory& f, std::ostream& os)
{
    save_factory_structure((Factory&)f, os);
}

void generate_simulation_turn_report(const Factory&, std::ostream&, Time)
{

}
