#include <M5Unified.h>
#include <hidcomposite.h>
#include "animation.h"

HIDcomposite device;

bool update = true;

int mode = 0;
bool copyed = false;

uint32_t circle_color_list[8] = {0xcc3300, 0xff6633, 0xffff66, 0x33cc33,
                                 0x00ffff, 0x0000ff, 0xff3399, 0x990099};

M5Canvas canvas(&M5.Display);
M5Canvas canvasTitle(&M5.Display);

uint8_t animation_index = 0;
const uint8_t *list[] = {
    Astronaut_1_0, Astronaut_1_1, Astronaut_1_2, Astronaut_1_3, Astronaut_1_4,
    Astronaut_1_5, Astronaut_1_6, Astronaut_1_7, Astronaut_1_8, Astronaut_1_9};

void setup()
{
  M5.begin();
  device.begin();
  M5.Display.setRotation(1);
  canvas.createSprite(64, 64);
  canvas.fillScreen(TFT_WHITE);
  canvasTitle.createSprite(128, 30);
}

void updateAnimation(bool pressed)
{
  canvas.setTextDatum(middle_center);
  canvas.drawJpg(list[animation_index], sizeof(Astronaut_1_0), 0, 0);
  if (animation_index < 9)
  {
    animation_index++;
  }
  else
  {
    animation_index = 0;
  }
  if (pressed)
  {
    canvas.pushSprite(32, 40);
    M5.Display.invertDisplay(true);
    delay(30);
  }
  else
  {
    canvas.pushSprite(32, 40);
    M5.Display.invertDisplay(false);
  }
}

bool sendPress(uint8_t _keycode1, uint8_t modifier)
{
  uint8_t keycode[6] = {0};
  keycode[0] = _keycode1;

  return tud_hid_keyboard_report(device.report_keyboard, modifier, keycode);
}

bool sendPress2(uint8_t _keycode1, uint8_t _keycode2, uint8_t modifier)
{
  uint8_t keycode[6] = {0};
  keycode[0] = _keycode1;
  keycode[1] = _keycode2;

  return tud_hid_keyboard_report(device.report_keyboard, modifier, keycode);
}

bool sendPress3(uint8_t _keycode1, uint8_t _keycode2, uint8_t _keycode3, uint8_t modifier)
{
  uint8_t keycode[6] = {0};
  keycode[0] = _keycode1;
  keycode[1] = _keycode2;
  keycode[2] = _keycode3;

  return tud_hid_keyboard_report(device.report_keyboard, modifier, keycode);
}

void drawTitle()
{
  canvasTitle.setFont(&fonts::DejaVu18);
  canvasTitle.setTextDatum(middle_center);
  canvasTitle.setTextColor(TFT_BLACK);
  canvasTitle.fillRect(0, 20, 128, 10,  circle_color_list[mode]);
  canvasTitle.fillRoundRect(0, 0, 128, 30, 8, circle_color_list[mode]);
  if (mode == 0)
  {
    canvasTitle.drawString("Lock", 128 / 2, 15);
  }
  else if (mode == 1)
  {
    canvasTitle.drawString("PageDn", 128 / 2, 15);
  }
  else if (mode == 2)
  {
    if (copyed)
    {
      canvasTitle.drawString("Paste", 128 / 2, 15);
    }
    else
    {
      canvasTitle.drawString("Copy", 128 / 2, 15);
    }
  }
  else if (mode == 3)
  {
    canvasTitle.drawString("TenM Mute", 128 / 2, 15);
  }
  else if (mode == 4)
  {
    canvasTitle.drawString("TaskMgr", 128 / 2, 15);
  }
  canvasTitle.pushSprite(0, 0);
}

void drawDisplay()
{
  M5.Display.clearDisplay();
  M5.Display.fillRoundRect(0, 10, 128, 118, 8, TFT_WHITE);
  M5.Display.display();
}

void loop()
{
  M5.update();

  if (update)
  {
    M5.Lcd.clearDisplay();
    drawDisplay();
    drawTitle();
    update = false;
  }

  if (M5.BtnA.wasHold())
  {
    mode = (mode + 1) % 5;
    update = true;
    copyed = false;
  }

  if (M5.BtnA.wasSingleClicked())
  {
    update = true;
    if (mode == 0)
    {
      sendPress2(HID_KEY_GUI_LEFT, HID_KEY_L, 0);
    }
    else if (mode == 1)
    {
      sendPress(HID_KEY_PAGE_DOWN, 0);
    }
    else if (mode == 2)
    {
      if (!copyed)
      {
        sendPress2(HID_KEY_CONTROL_LEFT, HID_KEY_C, 0);
        copyed = true;
      }
      else
      {
        sendPress2(HID_KEY_CONTROL_LEFT, HID_KEY_V, 0);
        copyed = false;
      }
    }
    else if (mode == 3)
    {
      sendPress2(HID_KEY_ALT_LEFT, HID_KEY_M, 0);
    }
    else if (mode == 4)
    {
      sendPress3(HID_KEY_CONTROL_LEFT, HID_KEY_SHIFT_LEFT, HID_KEY_ESCAPE, 0);
    }
    updateAnimation(true);
    device.sendRelease();
  }

  updateAnimation(false);
  M5.Display.display();
}