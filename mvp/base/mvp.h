#ifndef MVP_H
#define MVP_H

template<typename T_Model, typename T_View>
class MVP
{
public:
    class Presenter;

    class Model {
    public:
        virtual ~Model() {}
    };

    class View {
    public:
        virtual ~View() {}

        Presenter* presenter = nullptr;

        /**
         * @brief setPresenter must call this on view construct finish
         * @param presenter
         */
        void setPresenter(Presenter* presenter) {
            this->presenter = presenter;
        }
    };

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
};

#endif // MVP_H
