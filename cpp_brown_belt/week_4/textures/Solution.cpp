#include "Common.h"

using namespace std;

// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`

namespace Shapes {

class Shape : public IShape {
public:
  Shape() {}
  Shape(Point p, Size s, shared_ptr<ITexture> t) :
  position(p), size(s), texture(t) {} 
  // Возвращает точную копию фигуры.
  // Если фигура содержит текстуру, то созданная копия содержит ту же самую
  // текстуру. Фигура и её копия совместно владеют этой текстурой.
  std::unique_ptr<IShape> Clone() const override final {
    return make_unique<Shape>(this->position, this->size, this->texture);
  }

  void SetPosition(Point p) override final  {
    position = move(p);
  }
  Point GetPosition() const override final {
    return position;
  }

  void SetSize(Size s) override {
    size = s;
  }
  Size GetSize() const override {
    return size;
  }

  void SetTexture(std::shared_ptr<ITexture> t) override {
    texture = move(t);
  }
  ITexture* GetTexture() const override {
    return texture.get();
  }

  // Рисует фигуру на указанном изображении
  void Draw(Image& i) const override;

  virtual bool Checker(size_t x, size_t y) const {
    return true;
  }

  virtual ~Shape() = default;
protected:
  Point position;
  Size size;
  shared_ptr<ITexture> texture;
};

void Shape::Draw(Image& i) const {
  const auto MAX_X_POINT = i[0].size();
  const auto MAX_Y_POINT = i.size();

  // Check if position is inside the image
  if(position.x > MAX_X_POINT || position.y > MAX_Y_POINT) {
    return;
  }

  const size_t Y_END = (position.y + size.height);
  const size_t X_END = (position.x + size.width);

  if(texture.get() == nullptr) {
    for(size_t y = position.y; y <= MAX_Y_POINT && y < Y_END; y++) {
      for(size_t x = position.x; x <= MAX_X_POINT && x < X_END; x++) {
        i[y][x] = '.';
      }
    }
  } else {
    Size texture_size = texture->GetSize();
    const Image& texture_img = texture->GetImage();
    for(size_t y = position.y, t_y = 0; y < Y_END && y <= MAX_Y_POINT; y++, t_y++) {
      for(size_t x = position.x, t_x = 0; x < X_END && x <= MAX_X_POINT; x++, t_x++) {
        if(Checker(x,y)) {
          if(t_y < texture_size.height && t_x < texture_size.width) {
            i[y][x] = texture_img[t_y][t_x];
          } else {
            i[y][x] = '.';
          }
        }
      }
    }
  }
}

class Rectangle : public Shape {
};

class Ellipse : public Shape {
  virtual bool Checker(size_t x, size_t y) const override {
    return IsPointInEllipse(
    Point{static_cast<int>(x - this->position.x),static_cast<int>(y - this->position.y)},
    this->size);
  }
};

}

// Напишите реализацию функции
unique_ptr<IShape> MakeShape(ShapeType shape_type) {
  switch(shape_type) {
  case ShapeType::Rectangle :
    return make_unique<Shapes::Rectangle>();
  case ShapeType::Ellipse :
    return make_unique<Shapes::Ellipse>();
  default:
    throw invalid_argument("");
  }
}