#ifndef MVP_H
#define MVP_H

namespace MVP {
    class Model {
    public:
        virtual ~Model() {}
    };

    template<typename T_Presenter>
    class View {
    public:
        virtual ~View() {}

        void setPresenter(T_Presenter* presenter) {
            this->presenter = presenter;
        }

        T_Presenter* presenter = nullptr;
    };

    template<typename T_Model, typename T_View>
    class Presenter
    {
    public:
        explicit Presenter(T_View* view)
            :view(view)
        {}

        virtual ~Presenter() {}

        T_Model* model = nullptr;
        T_View* view = nullptr;
    };
}

#endif // MVP_H
