#ifdef _DEBUG
constexpr const bool NO_MOVE = false;
constexpr const bool NO_RESIZE = false;
constexpr const bool NO_COLLAPSE = true;
#else
constexpr const bool NO_MOVE = true;
constexpr const bool NO_RESIZE = true;
constexpr const bool NO_COLLAPSE = true;
#endif // _DEBUG