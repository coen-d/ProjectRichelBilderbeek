//---------------------------------------------------------------------------
/*
ImageCanvas, class to convert an image to ASCII art
Copyright (C) 2011-2014 Richel Bilderbeek

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
//---------------------------------------------------------------------------
//From http://www.richelbilderbeek.nl/CppImageCanvas.htm
//---------------------------------------------------------------------------
#ifndef IMAGECANVAS_H
#define IMAGECANVAS_H

#include <iosfwd>
#include <string>
#include <vector>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Weffc++"
#pragma GCC diagnostic ignored "-Wunused-local-typedefs"
#include <boost/checked_delete.hpp>
#include <boost/signals2.hpp>
#include "canvas.h"
#pragma GCC diagnostic pop

struct QImage;

namespace ribi {

///ImageCanvas converts an image to a Canvas
struct ImageCanvas : public Canvas
{
  ///The number of characters the Canvas is heigh and wide
  ///but also the maximum x and y coordinat. The minimum
  ///x and y coordinats are 0.0 and 0.0
  ImageCanvas(
    const std::string& filename,
    const int n_cols,
    const CanvasColorSystem colorSystem         = CanvasColorSystem::normal,
    const CanvasCoordinatSystem coordinatSystem = CanvasCoordinatSystem::screen);

  ///The color system used:
  ///- normal: full/drawn is displayed by M
  ///- invert: empty/non-drawn is displayed by M
  CanvasColorSystem GetColorSystem() const noexcept { return m_color_system; }

  ///The coordinat system used in displayal:
  ///- screen: origin is at top-left of the screen
  ///- graph: origin is at bottom-left of the screen
  CanvasCoordinatSystem GetCoordinatSystem() const noexcept { return m_coordinat_system; }

  int GetHeight() const noexcept;

  static const std::string GetVersion() noexcept;

  static const std::vector<std::string> GetVersionHistory() noexcept;

  int GetWidth() const noexcept { return m_n_cols; }

  ///Set the color system used
  void SetColorSystem(const CanvasColorSystem color_system) noexcept;

  ///Set the coordinat system used
  void SetCoordinatSystem(const CanvasCoordinatSystem coordinat_system) noexcept;

  const std::vector<std::string> ToStrings() const noexcept;

  ///This signal is emitted when any member variable changes
  boost::signals2::signal<void(ImageCanvas*)> m_signal_changed;

  private:
  ~ImageCanvas() noexcept {}
  friend void boost::checked_delete<>(ImageCanvas* x);

  ///Canvas is the original image its greynesses
  const std::vector<std::vector<double>> m_canvas;

  ///The color system used:
  ///- normal: full/drawn is displayed by M
  ///- invert: empty/non-drawn is displayed by M
  CanvasColorSystem m_color_system;

  ///The coordinat system used in displayal:
  ///- screen: origin is at top-left of the screen
  ///- graph: origin is at bottom-left of the screen
  CanvasCoordinatSystem m_coordinat_system;

  ///Number of columns
  const int m_n_cols;

  ///Returns a Y-X-ordered std::vector of greynesses, with the same size as the original image
  ///For example:
  ///
  /// a three pixel gradient line -> {0.0, 0.5, 1.0 }
  ///
  static const std::vector<std::vector<double> >
    ConvertToGreyYx(const QImage * const i);

  ///Returns a Y-X-ordered std::vector of greynesses, with the same size as the original image
  static const std::vector<std::vector<double> >
    ConvertToGreyYx(const std::string& filename);

  ///Converts a Y-X-ordered std::vector of greynesses
  ///to a text with a certain number of columns,
  ///For example:
  ///
  /// {0.0, 0.5, 1.0} -> " nM"
  ///
  ///'greynesses' must be a y-x-ordered std::vector of grey values
  ///ranging from [0.0,1.0], where 0.0 denotes black and
  ///1.0 denotes white.
  ///From http://www.richelbilderbeek.nl/CppImageToAscii.htm
  static const std::vector<std::string>
    ConvertGreynessesToAscii(const std::vector<std::vector<double> >& greynesses,
    const int n_cols);


  ///Generalizes a pixel, line or rectangle to one average greyness
  static double GetFractionGrey(
    const std::vector<std::vector<double> >& image,
    const int x1,
    const int y1,
    const int x2,
    const int y2) noexcept;

  ///
  static double GetGreyness(
    const std::vector<std::vector<double> >& image,
    const int x,
    const int y) noexcept;

  static double GetGreyness(
    const std::vector<std::vector<double> >& image,
    const int x1,
    const int x2,
    const int y) noexcept;

  //Get a square of pixels' average greyness
  static double GetGreyness(
    const std::vector<std::vector<double> >& image,
    const int x1,
    const int y1,
    const int x2,
    const int y2) noexcept;

  #ifndef NDEBUG
  static void Test() noexcept;
  #endif

  friend std::ostream& operator<<(std::ostream& os, const ImageCanvas& canvas);
};

std::ostream& operator<<(std::ostream& os, const ImageCanvas& canvas);

} //~namespace ribi

#endif