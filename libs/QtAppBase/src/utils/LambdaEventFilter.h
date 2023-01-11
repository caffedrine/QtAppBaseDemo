/**
  * https://gist.github.com/PsychedelicShayna/9cc2166ed5598456f88d78620bc64864
  */

#ifndef _LAMBDAEVENTFILTER_H_
#define _LAMBDAEVENTFILTER_H_

#include <QObject>
#include <QEvent>
#include <stdlib.h>
#include <experimental/tuple>

template<typename ...LARG>
class LambdaEventFilter : public QObject {
protected:
    struct AbstractLambdaContainer {
        virtual bool CallLambdaFunction(QObject*, QObject*, QEvent*) = 0;
        virtual ~AbstractLambdaContainer() = default;
    } *lambdaContainer;

    template<typename LF>
    class LambdaContainer : public AbstractLambdaContainer {
    protected:
        std::tuple<LARG...> lambdaArguments;
        LF lambdaFunction;

    public:
        bool CallLambdaFunction(QObject* parent, QObject* watched, QEvent* event) override {
            auto calling_arguments = std::tuple_cat(std::make_tuple(parent, watched, event), lambdaArguments);

            // TODO: FIX
            return std::experimental::apply(lambdaFunction, calling_arguments);
        }

        LambdaContainer(LF lambda_function, LARG... larg) : lambdaFunction(lambda_function) {
            lambdaArguments = std::make_tuple(larg...);
        }
    };

    QObject* parentQObject;

public:
    bool eventFilter(QObject* watched, QEvent* event) override {
        return lambdaContainer->CallLambdaFunction(parentQObject, watched, event);
    }

    template<typename LF>
    LambdaEventFilter(QObject* parent, LF lambda_function, LARG... larg) : parentQObject(parent) {
        lambdaContainer = new LambdaContainer<LF> { lambda_function, larg... };
    }

    ~LambdaEventFilter() override {
        if(lambdaContainer != nullptr) {
            delete lambdaContainer;
        }
    }
};

#endif // _LAMBDAEVENTFILTER_H_
