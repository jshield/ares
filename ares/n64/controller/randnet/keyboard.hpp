struct RandNetKeyboard : Controller {
  Node::Port port;
  Node::Peripheral layout;

  RandNetKeyboard(Node::Port);
  ~RandNetKeyboard();

  auto comm(n8 send, n8 recv, n8 input[], n8 output[]) -> n2 override;

private:
  Node::Input::Button matrix[85];
  static string labels[85];
  static u8 scancodes[85 * 2];
};
