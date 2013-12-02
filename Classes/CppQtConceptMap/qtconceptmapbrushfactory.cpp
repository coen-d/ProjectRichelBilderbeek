#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"
#include "qtconceptmapbrushfactory.h"

#include <QLinearGradient>
#pragma GCC diagnostic pop

const int ribi::cmap::QtConceptMapBrushFactory::sm_bottom = 10;
const int ribi::cmap::QtConceptMapBrushFactory::sm_left = -100;
const int ribi::cmap::QtConceptMapBrushFactory::sm_right = 100;
const int ribi::cmap::QtConceptMapBrushFactory::sm_top = -10;
const int ribi::cmap::QtConceptMapBrushFactory::sm_color_low_value  =  64;
const int ribi::cmap::QtConceptMapBrushFactory::sm_color_mid_value  = 128;

const std::string ribi::cmap::QtConceptMapBrushFactory::BrushToStr(const QBrush& brush)
{
  if (brush == CreateBlueGradientBrush()) return "blue";
  if (brush == CreateGoldGradientBrush()) return "gold";
  if (brush == CreateGrayGradientBrush()) return "gray";
  if (brush == CreateGreenGradientBrush()) return "green";
  if (brush == CreateRedGradientBrush()) return "red";
  if (brush == CreateYellowGradientBrush()) return "yellow";
  if (brush == CreateWhiteGradientBrush()) return "white";
  return "unknown";

}

const QBrush ribi::cmap::QtConceptMapBrushFactory::CreateBlueGradientBrush()
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor( sm_color_mid_value, sm_color_mid_value,255));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

const QBrush ribi::cmap::QtConceptMapBrushFactory::CreateGoldGradientBrush()
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor( sm_color_mid_value, sm_color_mid_value,sm_color_low_value));
  linearGradient.setColorAt(1.0,QColor(255,255,sm_color_low_value));
  return linearGradient;
}

const QBrush ribi::cmap::QtConceptMapBrushFactory::CreateGrayGradientBrush()
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor(196,196,196));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

const QBrush ribi::cmap::QtConceptMapBrushFactory::CreateGreenGradientBrush()
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor( sm_color_mid_value,255, sm_color_mid_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

const QBrush ribi::cmap::QtConceptMapBrushFactory::CreateRedGradientBrush()
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor(255, sm_color_mid_value, sm_color_mid_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

const QBrush ribi::cmap::QtConceptMapBrushFactory::CreateYellowGradientBrush()
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  linearGradient.setColorAt(0.0,QColor(255,255, sm_color_mid_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}

const QBrush ribi::cmap::QtConceptMapBrushFactory::CreateWhiteGradientBrush()
{
  QLinearGradient linearGradient(sm_left,sm_top,sm_right,sm_bottom);
  const int low_value = 250;
  linearGradient.setColorAt(0.0,QColor(low_value,low_value,low_value));
  linearGradient.setColorAt(1.0,QColor(255,255,255));
  return linearGradient;
}