#include <iostream>
#include <random>
#include <iomanip>
#include <fstream>

struct Interval {
  double a;
  double b;

  double Length() const {
    return b - a;
  }

  void Normalize() {
    if (a < b) {
      return;
    }
    a = b;
  }
};

struct Circle {
  double x;
  double y;
  double r;

  Circle() = delete;
  Circle(double x, double y, double r) : x(x), y(y), r(r) {
  }

  bool IsInside(double a, double b) const {
    return (a - x) * (a - x) + (b - y) * (b - y) <= r * r;
  }
};

std::pair<Interval, Interval> GetWideArea(const std::vector<Circle>& circles) {
  if (circles.empty()) {
    throw std::logic_error("Empty circles array");
  }
  Interval x_axis = {circles[0].x - circles[0].r, circles[0].x + circles[0].r};
  Interval y_axis = {circles[0].y - circles[0].r, circles[0].y + circles[0].r};
  for (const Circle& circle : circles) {
    x_axis.a = std::min(x_axis.a, circle.x - circle.r);
    x_axis.b = std::max(x_axis.b, circle.x + circle.r);
    y_axis.a = std::min(y_axis.a, circle.y - circle.r);
    y_axis.b = std::max(y_axis.b, circle.y + circle.r);
  }
  return {x_axis, y_axis};
}

std::pair<Interval, Interval> GetNarrowArea(const std::vector<Circle>& circles) {
  if (circles.empty()) {
    throw std::logic_error("Empty circles array");
  }
  Interval x_axis = {circles[0].x - circles[0].r, circles[0].x + circles[0].r};
  Interval y_axis = {circles[0].y - circles[0].r, circles[0].y + circles[0].r};
  for (const Circle& circle : circles) {
    x_axis.a = std::max(x_axis.a, circle.x - circle.r);
    x_axis.b = std::min(x_axis.b, circle.x + circle.r);
    y_axis.a = std::max(y_axis.a, circle.y - circle.r);
    y_axis.b = std::min(y_axis.b, circle.y + circle.r);
  }
  x_axis.Normalize();
  y_axis.Normalize();
  return {x_axis, y_axis};
}

double Rnd(const Interval& interval, std::mt19937& rnd) {
  return interval.a + static_cast<double>(rnd()) / rnd.max() * interval.Length();
}

void WriteDataIntoFile(const std::vector<Circle>& circles, const Interval& x_axis, const Interval& y_axis,
                        std::ofstream& out, std::mt19937& rnd, int points_count = 100'101) {
  int inside_counter = 0;
  for (int i = 0; i < points_count; ++i) {
    double x = Rnd(x_axis, rnd);
    double y = Rnd(y_axis, rnd);
    bool inside = true;
    for (const Circle& circle : circles) {
      if (!circle.IsInside(x, y)) {
        inside = false;
      }
    }
    inside_counter += inside;
    if ((i - 100) % 500 == 0) {
      out << i << " ";
      out << std::fixed << std::setprecision(20)
          << static_cast<double>(inside_counter) / i * (x_axis.Length() * y_axis.Length()) << "\n";
    }
  }
}

signed main() {
  std::vector<Circle> arr = {Circle(1, 1, 1), Circle(1.5, 2, sqrt(5) / 2), Circle(2, 1.5, sqrt(5) / 2)};

  std::mt19937 rnd(time(nullptr));
  auto [x_axis_wide, y_axis_wide] = GetWideArea(arr);
  auto [x_axis_narrow, y_axis_narrow] = GetNarrowArea(arr);
  for (int i = 0; i < 10; i++) {
    std::ofstream out;

    out.open("../wideAreaData/" + std::to_string(i + 1) + ".csv");
    WriteDataIntoFile(arr, x_axis_wide, y_axis_wide, out, rnd);
    out.close();

    out.open("../narrowAreaData/" + std::to_string(i + 1) + ".csv");
    WriteDataIntoFile(arr, x_axis_narrow, y_axis_narrow, out, rnd);
    out.close();
  }
  return 0;
}
