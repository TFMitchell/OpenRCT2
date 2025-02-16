/*****************************************************************************
 * Copyright (c) 2014-2020 OpenRCT2 developers
 *
 * For a complete list of all authors, please refer to contributors.md
 * Interested in contributing? Visit https://github.com/OpenRCT2/OpenRCT2
 *
 * OpenRCT2 is licensed under the GNU General Public License version 3.
 *****************************************************************************/

#pragma once

#ifdef ENABLE_SCRIPTING

#    include "../../../actions/GameAction.h"
#    include "../../../interface/Screenshot.h"
#    include "../../../localisation/Formatting.h"
#    include "../../../object/ObjectManager.h"
#    include "../../../scenario/Scenario.h"
#    include "../../Duktape.hpp"
#    include "../../HookEngine.h"
#    include "../../ScriptEngine.h"
#    include "../game/ScConfiguration.hpp"
#    include "../game/ScDisposable.hpp"
#    include "../object/ScObject.hpp"

#    include <cstdio>
#    include <memory>

namespace OpenRCT2::Scripting
{
    class ScContext
    {
    private:
        ScriptExecutionInfo& _execInfo;
        HookEngine& _hookEngine;

    public:
        ScContext(ScriptExecutionInfo& execInfo, HookEngine& hookEngine)
            : _execInfo(execInfo)
            , _hookEngine(hookEngine)
        {
        }

    private:
        int32_t apiVersion_get()
        {
            return OPENRCT2_PLUGIN_API_VERSION;
        }

        std::shared_ptr<ScConfiguration> configuration_get()
        {
            return std::make_shared<ScConfiguration>();
        }

        std::shared_ptr<ScConfiguration> sharedStorage_get()
        {
            auto& scriptEngine = GetContext()->GetScriptEngine();
            return std::make_shared<ScConfiguration>(scriptEngine.GetSharedStorage());
        }

        void captureImage(const DukValue& options)
        {
            auto ctx = GetContext()->GetScriptEngine().GetContext();
            try
            {
                CaptureOptions captureOptions;
                captureOptions.Filename = fs::u8path(AsOrDefault(options["filename"], ""));
                captureOptions.Rotation = options["rotation"].as_int() & 3;
                captureOptions.Zoom = ZoomLevel(options["zoom"].as_int());
                captureOptions.Transparent = AsOrDefault(options["transparent"], false);

                auto dukPosition = options["position"];
                if (dukPosition.type() == DukValue::Type::OBJECT)
                {
                    CaptureView view;
                    view.Width = options["width"].as_int();
                    view.Height = options["height"].as_int();
                    view.Position.x = dukPosition["x"].as_int();
                    view.Position.y = dukPosition["y"].as_int();
                    captureOptions.View = view;
                }

                CaptureImage(captureOptions);
            }
            catch (const DukException&)
            {
                duk_error(ctx, DUK_ERR_ERROR, "Invalid options.");
            }
            catch (const std::exception& ex)
            {
                duk_error(ctx, DUK_ERR_ERROR, ex.what());
            }
        }

        static DukValue CreateScObject(duk_context* ctx, ObjectType type, int32_t index)
        {
            switch (type)
            {
                case ObjectType::Ride:
                    return GetObjectAsDukValue(ctx, std::make_shared<ScRideObject>(type, index));
                case ObjectType::SmallScenery:
                    return GetObjectAsDukValue(ctx, std::make_shared<ScSmallSceneryObject>(type, index));
                default:
                    return GetObjectAsDukValue(ctx, std::make_shared<ScObject>(type, index));
            }
        }

        DukValue getObject(const std::string& typez, int32_t index) const
        {
            auto ctx = GetContext()->GetScriptEngine().GetContext();
            auto& objManager = GetContext()->GetObjectManager();

            auto type = ScObject::StringToObjectType(typez);
            if (type)
            {
                auto obj = objManager.GetLoadedObject(*type, index);
                if (obj != nullptr)
                {
                    return CreateScObject(ctx, *type, index);
                }
            }
            else
            {
                duk_error(ctx, DUK_ERR_ERROR, "Invalid object type.");
            }
            return ToDuk(ctx, nullptr);
        }

        std::vector<DukValue> getAllObjects(const std::string& typez) const
        {
            auto ctx = GetContext()->GetScriptEngine().GetContext();
            auto& objManager = GetContext()->GetObjectManager();

            std::vector<DukValue> result;
            auto type = ScObject::StringToObjectType(typez);
            if (type)
            {
                auto count = object_entry_group_counts[EnumValue(*type)];
                for (int32_t i = 0; i < count; i++)
                {
                    auto obj = objManager.GetLoadedObject(*type, i);
                    if (obj != nullptr)
                    {
                        result.push_back(CreateScObject(ctx, *type, i));
                    }
                }
            }
            else
            {
                duk_error(ctx, DUK_ERR_ERROR, "Invalid object type.");
            }
            return result;
        }

        int32_t getRandom(int32_t min, int32_t max)
        {
            ThrowIfGameStateNotMutable();
            if (min >= max)
                return min;
            int32_t range = max - min;
            return min + scenario_rand_max(range);
        }

        duk_ret_t formatString(duk_context* ctx)
        {
            auto nargs = duk_get_top(ctx);
            if (nargs >= 1)
            {
                auto dukFmt = DukValue::copy_from_stack(ctx, 0);
                if (dukFmt.type() == DukValue::Type::STRING)
                {
                    FmtString fmt(dukFmt.as_string());

                    std::vector<FormatArg_t> args;
                    for (duk_idx_t i = 1; i < nargs; i++)
                    {
                        auto dukArg = DukValue::copy_from_stack(ctx, i);
                        switch (dukArg.type())
                        {
                            case DukValue::Type::NUMBER:
                                args.push_back(dukArg.as_int());
                                break;
                            case DukValue::Type::STRING:
                                args.push_back(dukArg.as_string());
                                break;
                            default:
                                duk_error(ctx, DUK_ERR_ERROR, "Invalid format argument.");
                                break;
                        }
                    }

                    auto result = FormatStringAny(fmt, args);
                    duk_push_lstring(ctx, result.c_str(), result.size());
                }
                else
                {
                    duk_error(ctx, DUK_ERR_ERROR, "Invalid format string.");
                }
            }
            else
            {
                duk_error(ctx, DUK_ERR_ERROR, "Invalid format string.");
            }
            return 1;
        }

        std::shared_ptr<ScDisposable> subscribe(const std::string& hook, const DukValue& callback)
        {
            auto& scriptEngine = GetContext()->GetScriptEngine();
            auto ctx = scriptEngine.GetContext();

            auto hookType = GetHookType(hook);
            if (hookType == HOOK_TYPE::UNDEFINED)
            {
                duk_error(ctx, DUK_ERR_ERROR, "Unknown hook type");
            }

            if (!callback.is_function())
            {
                duk_error(ctx, DUK_ERR_ERROR, "Expected function for callback");
            }

            auto owner = _execInfo.GetCurrentPlugin();
            if (owner == nullptr)
            {
                duk_error(ctx, DUK_ERR_ERROR, "Not in a plugin context");
            }

            auto cookie = _hookEngine.Subscribe(hookType, owner, callback);
            return std::make_shared<ScDisposable>([this, hookType, cookie]() { _hookEngine.Unsubscribe(hookType, cookie); });
        }

        void queryAction(const std::string& action, const DukValue& args, const DukValue& callback)
        {
            QueryOrExecuteAction(action, args, callback, false);
        }

        void executeAction(const std::string& action, const DukValue& args, const DukValue& callback)
        {
            QueryOrExecuteAction(action, args, callback, true);
        }

        void QueryOrExecuteAction(const std::string& actionid, const DukValue& args, const DukValue& callback, bool isExecute)
        {
            auto& scriptEngine = GetContext()->GetScriptEngine();
            auto ctx = scriptEngine.GetContext();
            try
            {
                auto action = scriptEngine.CreateGameAction(actionid, args);
                if (action != nullptr)
                {
                    auto plugin = scriptEngine.GetExecInfo().GetCurrentPlugin();
                    if (isExecute)
                    {
                        action->SetCallback(
                            [this, plugin, callback](const GameAction*, const GameActions::Result* res) -> void {
                                HandleGameActionResult(plugin, *res, callback);
                            });
                        GameActions::Execute(action.get());
                    }
                    else
                    {
                        auto res = GameActions::Query(action.get());
                        HandleGameActionResult(plugin, res, callback);
                    }
                }
                else
                {
                    duk_error(ctx, DUK_ERR_ERROR, "Unknown action.");
                }
            }
            catch (DukException&)
            {
                duk_error(ctx, DUK_ERR_ERROR, "Invalid action parameters.");
            }
        }

        void HandleGameActionResult(
            const std::shared_ptr<Plugin>& plugin, const GameActions::Result& res, const DukValue& callback)
        {
            // Construct result object
            auto& scriptEngine = GetContext()->GetScriptEngine();
            auto ctx = scriptEngine.GetContext();
            auto objIdx = duk_push_object(ctx);
            duk_push_int(ctx, static_cast<duk_int_t>(res.Error));
            duk_put_prop_string(ctx, objIdx, "error");

            if (res.Error != GameActions::Status::Ok)
            {
                auto title = res.GetErrorTitle();
                duk_push_string(ctx, title.c_str());
                duk_put_prop_string(ctx, objIdx, "errorTitle");

                auto message = res.GetErrorMessage();
                duk_push_string(ctx, message.c_str());
                duk_put_prop_string(ctx, objIdx, "errorMessage");
            }

            duk_push_int(ctx, static_cast<duk_int_t>(res.Cost));
            duk_put_prop_string(ctx, objIdx, "cost");

            duk_push_int(ctx, static_cast<duk_int_t>(res.Expenditure));
            duk_put_prop_string(ctx, objIdx, "expenditureType");

            auto args = DukValue::take_from_stack(ctx);

            if (callback.is_function())
            {
                // Call the plugin callback and pass the result object
                scriptEngine.ExecutePluginCall(plugin, callback, { args }, false);
            }
        }

        void registerAction(const std::string& action, const DukValue& query, const DukValue& execute)
        {
            auto& scriptEngine = GetContext()->GetScriptEngine();
            auto plugin = scriptEngine.GetExecInfo().GetCurrentPlugin();
            auto ctx = scriptEngine.GetContext();
            if (!query.is_function())
            {
                duk_error(ctx, DUK_ERR_ERROR, "query was not a function.");
            }
            else if (!execute.is_function())
            {
                duk_error(ctx, DUK_ERR_ERROR, "execute was not a function.");
            }
            else if (!scriptEngine.RegisterCustomAction(plugin, action, query, execute))
            {
                duk_error(ctx, DUK_ERR_ERROR, "action has already been registered.");
            }
        }

        int32_t SetIntervalOrTimeout(DukValue callback, int32_t delay, bool repeat)
        {
            auto& scriptEngine = GetContext()->GetScriptEngine();
            auto ctx = scriptEngine.GetContext();
            auto plugin = scriptEngine.GetExecInfo().GetCurrentPlugin();

            int32_t handle = 0;
            if (callback.is_function())
            {
                handle = scriptEngine.AddInterval(plugin, delay, repeat, std::move(callback));
            }
            else
            {
                duk_error(ctx, DUK_ERR_ERROR, "callback was not a function.");
            }
            return handle;
        }

        void ClearIntervalOrTimeout(int32_t handle)
        {
            auto& scriptEngine = GetContext()->GetScriptEngine();
            auto plugin = scriptEngine.GetExecInfo().GetCurrentPlugin();
            scriptEngine.RemoveInterval(plugin, handle);
        }

        int32_t setInterval(DukValue callback, int32_t delay)
        {
            return SetIntervalOrTimeout(callback, delay, true);
        }

        int32_t setTimeout(DukValue callback, int32_t delay)
        {
            return SetIntervalOrTimeout(callback, delay, false);
        }

        void clearInterval(int32_t handle)
        {
            ClearIntervalOrTimeout(handle);
        }

        void clearTimeout(int32_t handle)
        {
            ClearIntervalOrTimeout(handle);
        }

    public:
        static void Register(duk_context* ctx)
        {
            dukglue_register_property(ctx, &ScContext::apiVersion_get, nullptr, "apiVersion");
            dukglue_register_property(ctx, &ScContext::configuration_get, nullptr, "configuration");
            dukglue_register_property(ctx, &ScContext::sharedStorage_get, nullptr, "sharedStorage");
            dukglue_register_method(ctx, &ScContext::captureImage, "captureImage");
            dukglue_register_method(ctx, &ScContext::getObject, "getObject");
            dukglue_register_method(ctx, &ScContext::getAllObjects, "getAllObjects");
            dukglue_register_method(ctx, &ScContext::getRandom, "getRandom");
            dukglue_register_method_varargs(ctx, &ScContext::formatString, "formatString");
            dukglue_register_method(ctx, &ScContext::subscribe, "subscribe");
            dukglue_register_method(ctx, &ScContext::queryAction, "queryAction");
            dukglue_register_method(ctx, &ScContext::executeAction, "executeAction");
            dukglue_register_method(ctx, &ScContext::registerAction, "registerAction");
            dukglue_register_method(ctx, &ScContext::setInterval, "setInterval");
            dukglue_register_method(ctx, &ScContext::setTimeout, "setTimeout");
            dukglue_register_method(ctx, &ScContext::clearInterval, "clearInterval");
            dukglue_register_method(ctx, &ScContext::clearTimeout, "clearTimeout");
        }
    };
} // namespace OpenRCT2::Scripting

#endif
