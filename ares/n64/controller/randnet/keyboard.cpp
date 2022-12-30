#define N64_NUMKEYS 85

u8 RandNetKeyboard::scancodes[N64_NUMKEYS*2] = {
      0x0A, 0x08,
      0x0B, 0x01,
      0x0A, 0x01,
      0x0B, 0x08,
      0x0A, 0x07,
      0x0B, 0x07,
      0x0A, 0x02,
      0x0B, 0x02,
      0x0A, 0x03,
      0x0B, 0x03,
      0x0A, 0x04,
      0x02, 0x03,
      0x0B, 0x06,
      0x0A, 0x05,
      0x0B, 0x05,
      0x02, 0x08,
      0x02, 0x07,
      0x0D, 0x05,
      0x0C, 0x05,
      0x05, 0x05,
      0x06, 0x05,
      0x07, 0x05,
      0x08, 0x05,
      0x09, 0x05,
      0x09, 0x06,
      0x08, 0x06,
      0x07, 0x06,
      0x06, 0x06,
      0x05, 0x06,
      0x0C, 0x06,
      0x0D, 0x06,
      0x0D, 0x01,
      0x0C, 0x01,
      0x05, 0x01,
      0x06, 0x01,
      0x07, 0x01,
      0x08, 0x01,
      0x09, 0x01,
      0x09, 0x04,
      0x08, 0x04,
      0x07, 0x04,
      0x06, 0x04,
      0x05, 0x04,
      0x0C, 0x04,
      0x04, 0x06,
      0x0F, 0x05,
      0x0D, 0x07,
      0x0C, 0x07,
      0x05, 0x07,
      0x06, 0x07,
      0x07, 0x07,
      0x08, 0x07,
      0x09, 0x07,
      0x09, 0x03,
      0x08, 0x03,
      0x07, 0x03,
      0x06, 0x03,
      0x0D, 0x04,
      0x0E, 0x01,
      0x0D, 0x08,
      0x0C, 0x08,
      0x05, 0x08,
      0x06, 0x08,
      0x07, 0x08,
      0x08, 0x08,
      0x09, 0x08,
      0x09, 0x02,
      0x08, 0x02,
      0x07, 0x02,
      0x10, 0x04,
      0x02, 0x04,
      0x0E, 0x06,
      0x11, 0x07,
      0x0F, 0x07,
      0x11, 0x05,
      0x10, 0x08,
      0x10, 0x02,
      0x06, 0x02,
      0x0E, 0x02,
      0x10, 0x06,
      0x10, 0x10, // confirm this one
      0x02, 0x06,
      0x02, 0x05,
      0x03, 0x05,
      0x04, 0x05};

  string labels[N64_NUMKEYS] = {"Escape/Cancel Key キャンセル",
                                "F1",
                                "F2",
                                "F3",
                                "F4",
                                "F5",
                                "F6",
                                "F7",
                                "F8",
                                "F9",
                                "F10",
                                "F11",
                                "F12",
                                "Num Lock",
                                "Japanese Key below Numlock LED",
                                "Japanese Key below Caps Lock LED",
                                "Japanese Key below Power LED",
                                "Japanese Key above Tab",
                                "Number 1",
                                "Number 2",
                                "Number 3",
                                "Number 4",
                                "Number 5",
                                "Number 6",
                                "Number 7",
                                "Number 8",
                                "Number 9",
                                "Number 0",
                                "- After Zero",
                                "^ Next to Backspace",
                                "Backspace 削除",
                                "Tab",
                                "Q",
                                "W",
                                "E",
                                "R",
                                "T",
                                "Y",
                                "U",
                                "I",
                                "O",
                                "P",
                                "'",
                                "{",
                                "}",
                                "Caps Lock 英数",
                                "A",
                                "S",
                                "D",
                                "F",
                                "G",
                                "H",
                                "J",
                                "K",
                                "L",
                                "+",
                                "*",
                                "Enter 改行　決定",
                                "LeftShift",
                                "Z",
                                "X",
                                "C",
                                "V",
                                "B",
                                "N",
                                "M",
                                "<",
                                ">",
                                "?",
                                "- (Long dash)",
                                "Up Cursor",
                                "Right Shift",
                                "Ctrl",
                                "Opt",
                                "| (Pipes)",
                                "Alt",
                                "Japanese 'alphanumeric key'",
                                "Space",
                                "Japanese 'kana'",
                                "Japanese Character",
                                "Home 行頭",
                                "End 行末",
                                "Left Cursor",
                                "Down Cursor",
                                "Right Cursor"};      

RandNetKeyboard::RandNetKeyboard(Node::Port parent)
{
  node = parent->append<Node::Peripheral>("Keyboard");
  // Define the keyboard matrix
  for (u32 kIndex : range(N64_NUMKEYS))
  {
    matrix[kIndex] = node->append<Node::Input::Button>(labels[kIndex]);
  }
}

RandNetKeyboard::~RandNetKeyboard() {
  
}

auto RandNetKeyboard::comm(n8 send, n8 recv, n8 input[], n8 output[]) -> n2
{
  b1 valid = 0;
  b1 over = 0;

  // status
  if (input[0] == 0x00 || input[0] == 0xff)
  {
    output[0] = 0x00; // 0x05 = gamepad; 0x02 = mouse
    output[1] = 0x02;
    output[2] = 0x00; // 0x02 = nothing present in controller slot
    valid = 1;
  }

  // read controller state
  if (input[0] == 0x13)
  {
    u32 offset = 0;
    for (u32 kIndex : range(N64_NUMKEYS))
    {
      if (auto node = matrix[kIndex])
      {
        platform->input(node);
        if (node->value())
        {
          output[offset] = scancodes[kIndex*2];
          output[offset + 1] = scancodes[kIndex*2+1];
          offset += 2;
          if (offset >= 6)
            break;
        }
      }
    }

    output[7] = 0;

    if (recv <= 4)
    {
      over = 0;
    }
    else
    {
      over = 1;
    }
    valid = 1;
  }

  n2 status = 0;
  status.bit(0) = valid;
  status.bit(1) = over;
  return status;
}
