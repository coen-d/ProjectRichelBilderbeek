#include "conceptmapcenternode.h"

#include <cassert>
#include <sstream>

#include "conceptmapconcept.h"

ribi::cmap::CenterNode::CenterNode(
    const boost::shared_ptr<ribi::cmap::Concept>& concept,
    const double x,
    const double y)
  : Node(concept,x,y)
{

}

bool ribi::cmap::IsCenterNode(const boost::shared_ptr<const ribi::cmap::Node> node) noexcept
{
  assert(node);
  return boost::dynamic_pointer_cast<const CenterNode>(node).get();
}

const std::string ribi::cmap::CenterNode::ToXml() const noexcept
{
  std::stringstream s;
  s << "<center_node>";
  s << GetConcept()->ToXml();
  s << "<x>" << GetX() << "</x>";
  s << "<y>" << GetY() << "</y>";
  s << "</center_node>";

  const std::string r = s.str();
  assert(r.size() >= 13);
  assert(r.substr(0,13) == std::string("<center_node>"));
  assert(r.substr(r.size() - 14,14) == std::string("</center_node>"));

  return r;
}

