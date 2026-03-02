#include <m3ds/M3DS.hpp>

int main() {
    // Initialises systems, sets bottom screen as console (configurable)
    M3DS::Init _ {};

    // Create a root node
    M3DS::Root root {};
    // Create a viewport for the top screen
    M3DS::Viewport* viewport = root.emplaceChild<M3DS::Viewport>(M3DS::Screen::top);

    // Create a panel to hold the UI elements
    M3DS::PanelContainer* panel = viewport->emplaceChild<M3DS::PanelContainer>();
    // Set minimum size to fill 3DS screen
    panel->setMinSize({400.f, 240.f});
    // Set the panel's style to be a dark grey BoxStyle
    panel->setStyle(M3DS::BoxStyle{ .colour = M3DS::Colours::dark_grey });

    // Create a vertical box container to hold both labels (spaces them vertically, filling to width)
    M3DS::VBoxContainer* vbox = panel->emplaceChild<M3DS::VBoxContainer>();

    // Create a heading label
    M3DS::Label* heading = vbox->emplaceChild<M3DS::Label>();
    // Set text colour
    heading->colour = M3DS::Colours::red;
    // Centre text horizontally
    heading->setJustify(M3DS::Label::Justify::centre);
    // Set text content
    heading->setText("Hello, M3DS World!");

    // Create a body label
    M3DS::Label* body = vbox->emplaceChild<M3DS::Label>();
    // Set it to expand to fill extra space within the VBoxContainer
    body->setFillSpace(true);
    // Set text content. Note the line breaks (Label does not auto-wrap text)
    body->setText(
        "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut\n"
        "labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco\n"
        "laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in\n"
        "voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat\n"
        "non proident, sunt in culpa qui officia deserunt mollit anim id est laborum."
    );
    
    // Create a footer label
    M3DS::Label* footer = vbox->emplaceChild<M3DS::Label>();
    // Set text colour
    footer->colour = M3DS::Colours::grey;
    // Centre text horizontally
    footer->setJustify(M3DS::Label::Justify::centre);
    // Set text content
    footer->setText("Press START to exit.");

    root.mainLoop([&](const float delta) {
        if (M3DS::Input::isKeyPressed(M3DS::Input::Key::start))
            root.exit();

        root.treeInput();
        root.treeUpdate(delta);
        root.treeDraw(M3DS::Draw::draw_2d);
    });
}
