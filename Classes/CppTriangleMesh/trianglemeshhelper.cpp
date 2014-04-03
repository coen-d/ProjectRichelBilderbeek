#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include "trianglemeshhelper.h"

#include <iomanip>
#include <iostream>
#include <sstream>

#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/math/constants/constants.hpp>
#include "Shiny.h"

#include "geometry.h"
#include "trace.h"
//#include "trianglemeshedge.h"
#include "trianglemeshface.h"
#include "trianglemeshhelper.h"
#include "trianglemeshpoint.h"
#include "trianglemeshpointfactory.h"
#include "xml.h"
#pragma GCC diagnostic pop

ribi::trim::Helper::Helper::Helper()
{
  #ifndef NDEBUG
  Test();
  #endif
}

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> ribi::trim::Helper::CalcCenter(
  const std::vector<boost::shared_ptr<Point>>& points
) const noexcept
{
  typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Value;
  using boost::geometry::get;

  assert(!points.empty());
  Value sum(0.0,0.0,0.0);
  for (const auto point: points)
  {
    assert(point);
    assert(point->GetCoordinat());
    assert(!std::isnan( get<0>(*point->GetCoordinat())));
    assert(!std::isnan( get<1>(*point->GetCoordinat())));
    assert(!point->CanGetZ() || !std::isnan(point->GetZ().value()));
    sum += point->GetCoordinat3D();
  }
  const double n { static_cast<double>(points.size()) };
  return sum / n;
}

std::vector<ribi::trim::Helper::Coordinat2D> ribi::trim::Helper::CalcProjection(
  const std::vector<boost::shared_ptr<const ribi::trim::Point>>& v) const
{
  return Geometry().CalcProjection(PointsToCoordinats3D(v));
}

ribi::trim::Winding ribi::trim::Helper::CalcWindingHorizontal(
  const std::vector<boost::shared_ptr<const Point>>& points
) const noexcept
{
  using boost::geometry::get;

  const int n_points { static_cast<int>(points.size()) };

  //Extract the points
  std::vector<Coordinat3D> coordinats;
  for (int i=0; i!=n_points; ++i)
  {
    Coordinat3D co(
      get<0>(*points[i]->GetCoordinat()),
      get<1>(*points[i]->GetCoordinat()),
      points[i]->GetZ().value()
    );
    //TRACE(co);
    coordinats.push_back(co);
  }

  assert(coordinats.size() == coordinats.size());

  const bool a { Geometry().IsClockwiseHorizontal(coordinats) };
  const bool b { Geometry().IsCounterClockwiseHorizontal(coordinats) };
  if ( a && !b) return Winding::clockwise;
  if (!a &&  b) return Winding::counter_clockwise;
  return Winding::indeterminate;
}

ribi::trim::Helper::FaceSet ribi::trim::Helper::CreateEmptyFaceSet() const noexcept
{
  ribi::trim::Helper::FaceSet s(OrderByIndex());
  return s;
}

std::set<
  boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>,
  std::function<
    bool(
      boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>,
      boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>
    )
  >
>
  ribi::trim::Helper::ExtractCoordinats(
  const std::vector<boost::shared_ptr<const Point>>& points
)
{
  PROFILE_FUNC();
  using boost::geometry::get;

  std::set<Coordinat3D,std::function<bool(Coordinat3D,Coordinat3D)>> s(
    [](const Coordinat3D& lhs, const Coordinat3D& rhs)
    {
      return ribi::trim::Less(lhs,rhs);
    }
  );
  for (const auto point: points)
  {
    if (!point->CanGetZ())
    {
      TRACE("Extract these coordinats later: the Face must be assigned to a Layer first");
    }
    assert(point->CanGetZ());
    const Coordinat3D c(
      point->GetCoordinat3D()
    );
    s.insert(s.begin(),c);
  }
  return s;
}

ribi::trim::Helper::Coordinat3dSet ribi::trim::Helper::ExtractCoordinats(const ribi::trim::Face& face)
{
  face.DoExtractCoordinats();
  return face.GetCoordinats();
}

double ribi::trim::Helper::GetAngle(const boost::shared_ptr<const Point> point) const noexcept
{
  return Geometry().GetAngle(
    boost::geometry::get<0>(*point->GetCoordinat()),
    boost::geometry::get<1>(*point->GetCoordinat())
  );
}

bool ribi::trim::Helper::IsClockwise(
  const std::vector<boost::shared_ptr<const Point>>& points,
  const Coordinat3D& observer) const noexcept
{
  //using boost::geometry::get;
  std::vector<Coordinat3D> coordinats;
  for (auto point: points)
  {
    assert(point);
    coordinats.push_back(point->GetCoordinat3D());
  }
  assert(Geometry().IsPlane(coordinats));
  return Geometry().IsClockwise(coordinats,observer);
}

bool ribi::trim::Helper::IsClockwiseHorizontal(
  const std::vector<boost::shared_ptr<Point>>& points
) const noexcept
{
  using boost::geometry::get;
  assert(points.size() == 3);
  assert(points[0]);
  assert(points[1]);
  assert(points[2]);
  assert(points[0]->GetCoordinat());
  assert(points[1]->GetCoordinat());
  assert(points[2]->GetCoordinat());
  const Coordinat3D center(CalcCenter(points));
  const auto center_x(boost::geometry::get<0>(center));
  const auto center_y(boost::geometry::get<1>(center));
  assert(!std::isnan(center_x));
  assert(!std::isnan(center_y));

  //const double pi  = boost::math::constants::pi<double>();
  //const double tau = boost::math::constants::two_pi<double>();
  const std::vector<double> angles {
    Geometry().GetAngle(
      get<0>(*points[0]->GetCoordinat()) - center_x,
      get<1>(*points[0]->GetCoordinat()) - center_y
    ),
    Geometry().GetAngle(
      get<0>(*points[1]->GetCoordinat()) - center_x,
      get<1>(*points[1]->GetCoordinat()) - center_y
    ),
    Geometry().GetAngle(
      get<0>(*points[2]->GetCoordinat()) - center_x,
      get<1>(*points[2]->GetCoordinat()) - center_y
    )
  };
  const bool a = Geometry().IsClockwise(angles[0],angles[1]);
  const bool b = Geometry().IsClockwise(angles[1],angles[2]);
  const bool is_clockwise { a && b };
  return is_clockwise;
}

bool ribi::trim::Helper::IsConvex(const std::vector<boost::shared_ptr<const ribi::trim::Point>>& points) const noexcept
{
  return Geometry().IsConvex(PointsToCoordinats3D(points));
}

bool ribi::trim::Helper::IsConvex(const std::vector<boost::shared_ptr<ribi::trim::Point>>& points) const noexcept
{
  if (points.size() == 3) return true;

  #ifndef NDEBUG
  assert(points.size() == 4);
  const bool verbose = false;
  if (verbose)
  {
    std::stringstream s;
    s << "{";
    for (auto point3d: points)
    {
      assert(point3d);
      s << (*point3d) << ",";
    }
    std::string po_str(s.str());
    po_str[po_str.size() - 1] = '}';
    TRACE(po_str);
  }
  #endif

  const auto const_points(AddConst(points));

  assert(points.size() == const_points.size());
  assert(boost::geometry::get<0>(*points[0]->GetCoordinat()) == boost::geometry::get<0>(*const_points[0]->GetCoordinat()));
  assert(boost::geometry::get<1>(*points[0]->GetCoordinat()) == boost::geometry::get<1>(*const_points[0]->GetCoordinat()));
  assert(points[0]->GetZ() == const_points[0]->GetZ());
  assert(boost::geometry::get<0>(*points[1]->GetCoordinat()) == boost::geometry::get<0>(*const_points[1]->GetCoordinat()));
  assert(boost::geometry::get<1>(*points[1]->GetCoordinat()) == boost::geometry::get<1>(*const_points[1]->GetCoordinat()));
  assert(points[1]->GetZ() == const_points[1]->GetZ());
  assert(boost::geometry::get<0>(*points[2]->GetCoordinat()) == boost::geometry::get<0>(*const_points[2]->GetCoordinat()));
  assert(boost::geometry::get<1>(*points[2]->GetCoordinat()) == boost::geometry::get<1>(*const_points[2]->GetCoordinat()));
  assert(points[2]->GetZ() == const_points[2]->GetZ());
  assert(boost::geometry::get<0>(*points[3]->GetCoordinat()) == boost::geometry::get<0>(*const_points[3]->GetCoordinat()));
  assert(boost::geometry::get<1>(*points[3]->GetCoordinat()) == boost::geometry::get<1>(*const_points[3]->GetCoordinat()));
  assert(points[3]->GetZ() == const_points[3]->GetZ());

  #ifndef NDEBUG
  for (auto point: const_points) { assert(point); }
  #endif

  const auto coordinats(PointsToCoordinats3D(const_points));

  assert(const_points.size() == coordinats.size());

  return Geometry().IsConvex(coordinats);
}

bool ribi::trim::Helper::IsCounterClockwise(
  const std::vector<boost::shared_ptr<const Point>>& points,
  const Coordinat3D& observer) const noexcept
{
  assert(Geometry().IsPlane(PointsToCoordinats3D(points)));
  return Geometry().IsCounterClockwise(PointsToCoordinats3D(points),observer);
}

bool ribi::trim::Helper::IsCounterClockwise(
  const std::vector<boost::shared_ptr<Point>>& points,
  const Coordinat3D& observer) const noexcept
{
  assert(Geometry().IsPlane(PointsToCoordinats3D(AddConst(points))));
  return Geometry().IsCounterClockwise(PointsToCoordinats3D(AddConst(points)),observer);
}

bool ribi::trim::Helper::IsHorizontal(const ribi::trim::Face& face) noexcept
{
  using boost::geometry::get;
  const bool answer_1 = face.GetOrientation() == FaceOrientation::horizontal;

  const auto coordinats(ExtractCoordinats(face));

  typedef std::set<Coordinat3D>::iterator Iterator;
  typedef std::pair<Iterator,Iterator> Pair;
  const Pair xs(
    std::minmax_element(coordinats.begin(),coordinats.end(),
      [](const Coordinat3D& lhs,const Coordinat3D& rhs)
      {
        return get<0>(lhs) < get<0>(rhs);
      }
    )
  );

  const double dx = std::abs(get<0>(*xs.first) - get<0>(*xs.second));

  const Pair ys {
    std::minmax_element(coordinats.begin(),coordinats.end(),
      [](const Coordinat3D& lhs,const Coordinat3D& rhs)
      {
        return get<1>(lhs) < get<1>(rhs);
      }
    )
  };

  const double dy = std::abs(get<1>(*ys.first) - get<1>(*ys.second));

  const Pair zs {
    std::minmax_element(coordinats.begin(),coordinats.end(),
      [](const Coordinat3D& lhs,const Coordinat3D& rhs)
      {
        return get<2>(lhs) < get<2>(rhs);
      }
    )
  };

  const double dz = std::abs(get<2>(*zs.first) - get<2>(*zs.second));
  //const double dz { std::abs((*zs.first).GetZ() - (*zs.second).GetZ()) };

  const bool answer_2
    = dz == 0.0 || (dz * 1000.0 < dx && dz * 1000.0 < dy);

  if (answer_1 != answer_2)
  {
    TRACE("ERROR");
  }

  assert(answer_1 == answer_2);
  return answer_1;
}

bool ribi::trim::Helper::IsPlane(
  const std::vector<boost::shared_ptr<const ribi::trim::Point>>& points
) const noexcept
{
  return Geometry().IsPlane(PointsToCoordinats3D(points));
}

bool ribi::trim::Helper::IsPlane(
  const std::vector<boost::shared_ptr<ribi::trim::Point>>& points
) const noexcept
{
  return Geometry().IsPlane(PointsToCoordinats3D(AddConst(points)));
}

bool ribi::trim::Helper::IsVertical(const ribi::trim::Face& face) noexcept
{
  const bool answer_1 = face.GetOrientation() == FaceOrientation::vertical;
  #ifndef NDEBUG
  const bool answer_2 = !IsHorizontal(face);
  assert(answer_1 == answer_2);
  #endif
  return answer_1;
}

void ribi::trim::Helper::MakeConvex(
  std::vector<boost::shared_ptr<ribi::trim::Point>>& v
) const noexcept
{
  #ifndef NDEBUG
  const bool verbose = true;
  for (auto p: v) { assert(p); }
  assert(!v.empty());
  assert(v.size() == 4);
  #endif

  if (Helper().IsConvex(v)) return;

  #ifndef NDEBUG
  static int cnt = 0;
  cnt = 0;

  static int call_cnt = -1;
  ++call_cnt;
  #endif

  std::sort(v.begin(),v.end(),Helper().OrderByX());

  while (1)
  {
    if (Helper().IsConvex(v))
    {
      break;
    }
    #ifndef NDEBUG
    const std::vector<boost::shared_ptr<ribi::trim::Point>> before(v);
    #endif

    std::next_permutation(v.begin(),v.end(),Helper().OrderByX());

    #ifndef NDEBUG
    const std::vector<boost::shared_ptr<ribi::trim::Point>> after(v);
    assert(before != after);
    if (verbose)
    {
      #ifndef FIX_ISSUE_168
      TRACE(ToStr(AddConst(v)));
      #endif //#ifndef FIX_ISSUE_168
      ++cnt;
      //assert(cnt != (4 * 3 * 2 * 1) + 2);
      if (cnt == 26) break;
    }
    #endif
  }

  if (Helper().IsConvex(v)) return;

  #ifndef NDEBUG
  if(!Helper().IsConvex(v))
  {
    TRACE("ERROR");
    TRACE(call_cnt);
    TRACE("TRY AGAIN");
    std::sort(v.begin(),v.end(),Helper().OrderByX());
    for (int i=0; i!=26; ++i)
    {
      std::next_permutation(v.begin(),v.end(),Helper().OrderByX());
      {
        std::stringstream s;
        assert(v.size() == 4);
        s << Helper().IsConvex(v) << ": "
          << Geometry().ToStr(v[0]->GetCoordinat3D()) << "->"<< v[0]->GetIndex() << ","
          << Geometry().ToStr(v[1]->GetCoordinat3D()) << "->"<< v[1]->GetIndex() << ","
          << Geometry().ToStr(v[2]->GetCoordinat3D()) << "->"<< v[2]->GetIndex() << ","
          << Geometry().ToStr(v[3]->GetCoordinat3D()) << "->"<< v[3]->GetIndex() << '\n'
        ;
        TRACE(s.str());
      }
      {
        std::stringstream t;
        const auto w(Helper().CalcProjection(AddConst(v)));
        assert(w.size() == 4);
        t << Geometry().IsConvex(w) << ": "
          << Geometry().ToStr(w[0]) << ","
          << Geometry().ToStr(w[1]) << ","
          << Geometry().ToStr(w[2]) << ","
          << Geometry().ToStr(w[3])
        ;
        TRACE(t.str());
      }
    }
    //assert(!"BREAK");

    //for (auto p: v) { TRACE(p); TRACE(*p); }

    #ifdef FIX_ISSUE_168
    assert(Helper().IsConvex(v));
    //std::exit(0);
    //return; //TODO: FIX THIS BUG
    #endif //ifdef FIX_ISSUE_168
    assert(!"Should not get here");
  }
  #endif

  #ifdef FIX_ISSUE_168
  return; //TODO: FIX THIS BUG
  #endif //#ifdef FIX_ISSUE_168
  assert(Helper().IsConvex(v));
}

std::function<
    bool(
      const boost::shared_ptr<const ribi::trim::Face>& lhs,
      const boost::shared_ptr<const ribi::trim::Face>& rhs
    )
  >
  ribi::trim::Helper::OrderByIndex() const noexcept
{
  return [](const boost::shared_ptr<const Face>& lhs, const boost::shared_ptr<const Face>& rhs)
  {
    using boost::geometry::get;
    assert(lhs);
    assert(rhs);
    return lhs->GetIndex() < rhs->GetIndex();
  };

}

std::function<
    bool(
      const boost::shared_ptr<const ribi::trim::Point>& lhs,
      const boost::shared_ptr<const ribi::trim::Point>& rhs
    )
  >
  ribi::trim::Helper::OrderByX() const noexcept
{
  return [](const boost::shared_ptr<const Point>& lhs, const boost::shared_ptr<const Point>& rhs)
  {
    using boost::geometry::get;
    assert(lhs);
    assert(rhs);
    assert(lhs->GetCoordinat());
    assert(rhs->GetCoordinat());
    if (get<0>(*lhs->GetCoordinat()) < get<0>(*rhs->GetCoordinat())) return true;
    if (get<0>(*lhs->GetCoordinat()) > get<0>(*rhs->GetCoordinat())) return false;
    if (get<1>(*lhs->GetCoordinat()) < get<1>(*rhs->GetCoordinat())) return true;
    if (get<1>(*lhs->GetCoordinat()) > get<1>(*rhs->GetCoordinat())) return false;
    assert(lhs->CanGetZ());
    assert(rhs->CanGetZ());
    return lhs->GetZ() < rhs->GetZ();
  };
}

std::vector<boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>>
  ribi::trim::Helper::PointsToCoordinats3D(
    const std::vector<boost::shared_ptr<const ribi::trim::Point>>& points
) const noexcept
{
  typedef boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> Coordinat3D;
  std::vector<Coordinat3D> v;
  for (auto p: points)
  {
    assert(p);
    assert(p->GetCoordinat());
    v.push_back(p->GetCoordinat3D());
  }
  return v;
}

#ifndef NDEBUG
void ribi::trim::Helper::Test() noexcept
{
  {
    static bool is_tested = false;
    if (is_tested) return;
    is_tested = true;
  }
  TRACE("Starting ribi::trim::Helper::Point::Test");
  typedef boost::geometry::model::d2::point_xy<double> Coordinat2D;
  using boost::geometry::get;
  const bool verbose = true;

  //CalcCenter
  //CalcWindingHorizontal
  const Helper h;
  const double pi { boost::math::constants::pi<double>() };
  if (verbose) { TRACE("GetAngle"); }
  {
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(0.0,-1.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle = h.GetAngle(point); //North
      const double expected = 0.0 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(1.0,-1.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle =  h.GetAngle(point); //North-East
      const double expected = 0.25 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(1.0,0.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle =  h.GetAngle(point); //East
      const double expected = 0.5 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(1.0,1.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle =  h.GetAngle(point); //South-East
      const double expected = 0.75 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(0.0,1.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle =  h.GetAngle(point); //South
      const double expected = 1.0 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(-1.0,1.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle =  h.GetAngle(point); //South-West
      const double expected = 1.25 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(-1.0,0.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle =  h.GetAngle(point); //West
      const double expected = 1.5 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
    {
      const boost::shared_ptr<const Coordinat2D> coordinat {
        new Coordinat2D(-1.0,-1.0)
      };
      const boost::shared_ptr<Point> point {
        PointFactory().Create(coordinat)
      };
      const double angle =  h.GetAngle(point); //North-West
      const double expected = 1.75 * pi;
      assert(std::abs(angle-expected) < 0.01);
    }
  }
  //IsClockwiseHorizontal 1
  //IsClockwiseHorizontal 2
  //IsClockwiseVertical
  if (verbose) { TRACE("IsConvex, 2D, from error"); }
  {

    std::vector<boost::shared_ptr<const Coordinat2D>> coordinats2d;
    {
      const auto coordinat
        = boost::make_shared<Coordinat2D>(9.2885,29.5639);
      assert(coordinat);
      coordinats2d.push_back(coordinat);
    }
    {
      const auto coordinat
        = boost::make_shared<Coordinat2D>(9.2885,40.6764);
      assert(coordinat);
      coordinats2d.push_back(coordinat);
    }
    {
      const auto coordinat
        = boost::make_shared<Coordinat2D>(17.497,44.4009);
      assert(coordinat);
      coordinats2d.push_back(coordinat);
    }
    {
      const auto coordinat
        = boost::make_shared<Coordinat2D>(17.497,33.0765);
      assert(coordinat);
      coordinats2d.push_back(coordinat);
    }
    std::vector<boost::shared_ptr<Point>> points;
    for (const auto coordinat2d: coordinats2d)
    {
      const auto point(PointFactory().Create(coordinat2d));
      assert(point);
      point->SetZ(0.0 * boost::units::si::meter);
      points.push_back(point);
    }
    assert(coordinats2d.size() == points.size());
    assert(h.IsConvex(points));
    assert(h.IsConvex(AddConst(points)));
  }
  if (verbose) { TRACE("IsConvex, 3D, from error"); }
  {

    std::vector<boost::shared_ptr<const Coordinat3D>> coordinats3d;
    {
      const auto coordinat
        = boost::make_shared<Coordinat3D>(0.0,2.0,1.0);
      assert(coordinat);
      coordinats3d.push_back(coordinat);
    }
    {
      const auto coordinat
        = boost::make_shared<Coordinat3D>(0.0,2.0,2.0);
      assert(coordinat);
      coordinats3d.push_back(coordinat);
    }
    {
      const auto coordinat
        = boost::make_shared<Coordinat3D>(1.0,1.0,2.0);
      assert(coordinat);
      coordinats3d.push_back(coordinat);
    }
    std::vector<boost::shared_ptr<Point>> points;
    for (auto coordinat: coordinats3d)
    {
      boost::shared_ptr<const Coordinat2D> coordinat2d(
        new Coordinat2D(
          boost::geometry::get<0>(*coordinat),
          boost::geometry::get<1>(*coordinat)
        )
      );
      const auto point(PointFactory().Create(coordinat2d));
      assert(point);
      point->SetZ(boost::geometry::get<2>(*coordinat) * boost::units::si::meter);
      points.push_back(point);
    }
    assert(points.size() == coordinats3d.size());
    assert(h.IsConvex(points));
    assert(h.IsConvex(AddConst(points)));
  }

  //SetWindingHorizontal
  if (verbose) { TRACE("MakeConvex"); }
  {
    std::vector<boost::shared_ptr<const Coordinat3D>> coordinats3d;
    {
      const auto coordinat
        = boost::make_shared<Coordinat3D>(2.23114,3.23607,6);
      assert(coordinat);
      coordinats3d.push_back(coordinat);
    }
    {
      const boost::shared_ptr<const Coordinat3D> coordinat(new Coordinat3D(2.23114,3.23607,5));
      assert(coordinat);
      coordinats3d.push_back(coordinat);
    }
    {
      const boost::shared_ptr<const Coordinat3D> coordinat(new Coordinat3D(1.17557,2.35781,6));
      assert(coordinat);
      coordinats3d.push_back(coordinat);
    }
    {
      const boost::shared_ptr<const Coordinat3D> coordinat(new Coordinat3D(1.17557,2.35781,5));
      assert(coordinat);
      coordinats3d.push_back(coordinat);
    }

    std::vector<boost::shared_ptr<Point>> points;
    for (auto coordinat: coordinats3d)
    {
      boost::shared_ptr<const Coordinat2D> coordinat2d(
        new Coordinat2D(
          boost::geometry::get<0>(*coordinat),
          boost::geometry::get<1>(*coordinat)
        )
      );
      const auto point(PointFactory().Create(coordinat2d));
      assert(point);
      point->SetZ(boost::geometry::get<2>(*coordinat) * boost::units::si::meter);
      points.push_back(point);
    }
    assert(points.size() == coordinats3d.size());

    h.MakeConvex(points);
    #ifndef NDEBUG
    if(!h.IsConvex(points))
    {
      TRACE("ERROR");
    }
    #endif
    assert(h.IsConvex(points));
  }
  TRACE("Finished ribi::trim::Helper::Point::Test successfully");
}
#endif

std::string ribi::trim::Helper::ToStr(
  const std::vector<boost::shared_ptr<const Point>>& points
  ) const noexcept
{
  std::stringstream s;
  s << " {";
  for (auto point:points) { s << point->GetIndex() << ","; }
  std::string t(s.str());
  t[ t.size() - 1] = '}';
  return t;
}

std::string ribi::trim::Helper::ToXml(const boost::geometry::model::d2::point_xy<double>& p) const noexcept
{
  using boost::geometry::get;
  std::stringstream s;
  s
    << ribi::xml::ToXml("0",get<0>(p))
    << ribi::xml::ToXml("1",get<1>(p))
  ;

  std::stringstream t;
  t << ribi::xml::ToXml("Coordinat2D",s.str());
  return t.str();
}

bool ribi::trim::operator<(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs) noexcept
{
  using boost::geometry::get;
  if (get<0>(lhs) < get<0>(rhs)) return true;
  if (get<0>(lhs) > get<0>(rhs)) return false;

  if (get<1>(lhs) < get<1>(rhs)) return true;
  if (get<1>(lhs) > get<1>(rhs)) return false;

  return get<2>(lhs) < get<2>(rhs);
}

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> ribi::trim::operator+(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept
{
  using boost::geometry::get;
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> result(
    get<0>(lhs) + get<0>(rhs),
    get<1>(lhs) + get<1>(rhs),
    get<2>(lhs) + get<2>(rhs)
  );
  assert(std::abs(get<0>(result) - (get<0>(lhs) + get<0>(rhs))) < 0.001);
  assert(std::abs(get<1>(result) - (get<1>(lhs) + get<1>(rhs))) < 0.001);
  assert(std::abs(get<2>(result) - (get<2>(lhs) + get<2>(rhs))) < 0.001);
  return result;
}

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& ribi::trim::operator+=(
  boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept
{
  lhs = lhs + rhs;
  return lhs;
}

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& ribi::trim::operator/=(
  boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p,
  const double factor
) noexcept
{
  p = p / factor;
  return p;
}

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> ribi::trim::operator/(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& p,
  const double factor
) noexcept
{
  using boost::geometry::get;
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> result(
    get<0>(p) / factor,
    get<1>(p) / factor,
    get<2>(p) / factor
  );
  assert(std::abs(get<0>(result) - (get<0>(p) / factor)) < 0.001);
  assert(std::abs(get<1>(result) - (get<1>(p) / factor)) < 0.001);
  assert(std::abs(get<2>(result) - (get<2>(p) / factor)) < 0.001);
  return result;
}

boost::geometry::model::point<double,3,boost::geometry::cs::cartesian> ribi::trim::Add(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept { return lhs + rhs; }

bool ribi::trim::Less(
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& lhs,
  const boost::geometry::model::point<double,3,boost::geometry::cs::cartesian>& rhs
) noexcept
{
  return ribi::trim::operator <(lhs,rhs);
  //return lhs < rhs;
}

