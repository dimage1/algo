#include "precomp.h"

// aux types
using PopName = std::string;
enum PopState {
    Ready = 0,
    Stopped,
    Maintenance,

    Unknown
};
using PopMap = std::unordered_map<PopName, PopState>;
using StateFunc = std::function<void(const PopName&, const PopState&)>;

// states listener
class IPopStatesListener {
public:
    virtual void OnStateModified(const PopName &pop, const PopState &state) = 0;
};

// storage class
class PopStates {
public:
    static PopStates& instance() { return m_Instance; }

    void SetPopState(const PopName &pop, const PopState &state) {
        {
            const std::lock_guard<std::mutex> lock(m_Mutex);
            m_Pops[pop] = state;
        }

        for (const auto &l: m_StateUpdateListeners) {
            l->OnStateModified(pop, state);
        }
    }

    void GetPopState(const PopName &pop, PopState &state) const {
        const std::lock_guard<std::mutex> lock(m_Mutex);

        auto iter = m_Pops.find(pop);
        if (iter == m_Pops.end()) {
            state = PopState::Unknown;
        }
        else {
            state = iter->second;
        }
    }

    void ForAllPopStates (StateFunc fProcessState) {
        for (const auto p: m_Pops) {
            fProcessState(p.first, p.second);
        }
    }

    void AddStateUpdateListener(IPopStatesListener* listener) {
        m_StateUpdateListeners.insert(listener);
    }
    void RemoveStateUpdateListener(IPopStatesListener* listener) {
        m_StateUpdateListeners.erase(listener);
    }

private:
    PopStates() = default;

private:
    static PopStates m_Instance;
    PopMap m_Pops;

    std::set<IPopStatesListener*> m_StateUpdateListeners;
    mutable std::mutex m_Mutex;
};

// declare instances
PopStates PopStates::m_Instance;

class SimpleListener : public IPopStatesListener {
public:
    SimpleListener() = default;
    virtual ~SimpleListener() = default;

    void OnStateModified(const PopName &pop, const PopState &state) final {
        std::cout << pop << " = " << state << std::endl;
    }
};

int main(int argc, char **argv)
{
    PopStates &states = PopStates::instance();

    auto listener = [](const PopName& n, const PopState& s){
        std::cout << n << " = " << s << std::endl;
    };

    SimpleListener l;
    states.AddStateUpdateListener(&l);

    states.SetPopState("SVX", PopState::Ready);
    states.SetPopState("EXP", PopState::Stopped);

    states.RemoveStateUpdateListener(&l);
    states.SetPopState("GVA", PopState::Unknown);

    states.AddStateUpdateListener(&l);
    states.SetPopState("CDG", PopState::Ready);

    std::cout << "\n/!\\ All POP states:\n" ;
    states.ForAllPopStates([](const PopName& n, const PopState& s){
        std::cout << n << " = " << s << std::endl;
    });

    return EXIT_SUCCESS;
}
