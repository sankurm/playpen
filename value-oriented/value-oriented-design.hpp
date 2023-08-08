#include <optional>
#include <expected>
#include <variant>
#include <chrono>

struct tyre {};

enum tyre_pos { left_front, right_front, left_back, right_back };
struct car
{
    Onwer_id owner;
    tyre lf, rf, lb, rb;
};

bool is_punctured(tyre t);

bool fix_puncture(car* c, tyre_pos pos) {
    auto& tyre_to_fix = c->tyres[pos];
    //fix it
}

bool fix_puncture(car& c, tyre_pos pos) {
    auto& tyre_to_fix = c.tyres[pos];
    //fix it
}

//tyre fix_puncture(tyre t) {
std::optional<tyre> fix_puncture(tyre&& t) {
    tyre new_tyre;
    //fix it
    std::ranges::for_each(c.tyres, inflate);
    return new_tyre;
}

void program() {
    tyre left_back;
    std::optional<tyre> fixed = fix_puncture(std::move(left_back))
        .and_then(std::bind_back(inflate, 32_psi))
        .transform(wash)
        .and_then(polish)
        .value_or(tyre{});
    if (fixed) { left_back = fixed.value(); }
}

struct address {};
struct employee
{
    address home_addr;
};

bool validate_address(employee& e1) {
    address& a = e1.home_addr;
    //validate pin code, state, city, lane
}

#include <vector>
std::vector<int> create()
{
    std::vector<int> vi{3, 6, 9, 12, 15};
    for (auto &i : vi) { i /= 3; }
    return vi;
}
int main(int, char**) {
    auto vi = create();
    return vi.size();
}

struct Point2D { int x, y; };

class PointTwoD
{
    PointTwoD() = default;
    PointTwoD(int, int);

    bool operator==(const PointTwoD&) = default;

    private:
    int x, y;
};

struct Point
{
    int x, y;
    Point(const Point&);
};

bool operator==(const Point&, const Point&);

auto event = receive();
auto& group_chat = find(all_chats, event.chat_id);
process(group_chat, event);

car create_car(...);

car mycar;
void create_car(&mycar, ...);

//In fact - we should be able to use it like ints!
car mycar(...);

std::unique_ptr<car> make_car(...);

class Car
{

    //construction, destructor, special functions
};

class Car
{
    Onwer* owner;
    tyre *lf, *rf, *lb, *rb;

    //construction, destructor, special functions
};

struct timer{};
struct socket{};
struct codec{};
struct streamer{};
struct event{};

template<typename...Func>
struct overload : Func... {
    using Func::operator()...;
};

template<typename...Func> overload(Func...) -> overload<Func...>;

class Call
{
    //unconnected
    std::chrono::time_point last_conn;
    timer reconnection_timer;

    //active
    socket remote;

    //transmitting
    codec audio_codec;
    timer keepalive_timer;

    //on-hold
    streamer music_stream;
};

struct unconnected
{
    std::chrono::time_point last_conn;
    timer reconnection_timer;
};
struct active_tx
{
    socket remote;
    codec audio_codec;
    timer keepalive_timer;
};
struct active_on_hold
{
    socket remote;
    streamer music_stream;
};

using call_state = std::variant<unconnected, active_tx, active_on_hold>;
call_state sm;
class Call
{
    call_state state;
};
Call call;
call.state;

call_state handle(unconnected, event);
call_state handle(active_tx, event);
call_state handle(active_on_hold, event);

overload ov {
    [](unconnected state) { return handle(state, ev); }
    [](active_tx state) { return handle(state, ev); }
    [](active_on_hold state) { return handle(state, ev); }
};