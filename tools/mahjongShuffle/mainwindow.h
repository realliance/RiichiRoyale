#pragma once

#include <gtkmm/button.h>
#include <gtkmm/window.h>
#include <gtkmm/image.h>
#include <gtkmm/grid.h>
#include <gtkmm/checkbutton.h>
#include <gtkmm/label.h>
#include <vector>

#include "walls.h"

class MainWindow : public Gtk::Window
{

public:
  MainWindow();
  virtual ~MainWindow();

protected:
  //Signal handlers:
  void on_button_clicked();

  Walls walls;

  //Member widgets:
  Gtk::Grid grid;
  Gtk::Button button;
  Gtk::CheckButton stdformbutton;
  std::vector<Gtk::Image> imageArray;
  Glib::RefPtr<Gdk::Pixbuf> front;
  Gtk::CheckButton sortButton;
  Gtk::CheckButton stopButton;
  Gtk::CheckButton loopButton;
  Gtk::Label isStdForm;
  int dots = 0;

  private:
  auto getFilePath(Piece p) -> std::string;
  std::string path;
};
