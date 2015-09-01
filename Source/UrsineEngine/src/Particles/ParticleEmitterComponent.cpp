#include "UrsinePrecompiled.h"

#include "ParticleEmitterComponent.h"
#include "ParticleEmitterSystem.h"
#include "ParticleEffectSerializer.h"

namespace ursine
{
    namespace ecs
    {
        ParticleEmitter::ParticleEmitter(void)
            : BaseComponent()
            , _rate(1)
            , _interval(TimeSpan::FromSeconds(1.0f))
            , _type(INTERVAL_COMPLETED)
            , _timer(TimeSpan::FromSeconds(1.0f))
            , _last_spawned(TimeSpan::FromSeconds(1.0f))
            , _emit(true)
        {

        }

        ParticleEmitter::ParticleEmitter(const ParticleEmitter &other)
            : BaseComponent()
            , _interval(0)
            , _timer(0)
            , _last_spawned(0)
        {
            copy(other);
        }

        ParticleEmitter::ParticleEmitter(uint rate, TimeSpan interval, IntervalType type)
            : BaseComponent()
            , _rate(rate)
            , _interval(interval)
            , _type(type)
            , _timer(interval)
            , _last_spawned(interval)
        {

        }

        ParticleEmitter::~ParticleEmitter(void)
        {

        }

        const ParticleEmitter &ParticleEmitter::operator=(const ParticleEmitter &rhs)
        {
            copy(rhs);

            return *this;
        }

        void ParticleEmitter::OnInitialize(void)
        {
            // set all effect's this pointers
            for (auto &effect : _effects)
            {
                effect->_emitter = this;
                utils::FlagSet(_mask, effect->GetTypeMask());
                effect->OnInitialize();
            }
        }

        void ParticleEmitter::SetColor(const Color &color)
        {
            _color = color;
        }

        void ParticleEmitter::SetEmit(bool flag)
        {
            _emit = flag;
        }

        void ParticleEmitter::EmitParticles(uint count)
        {
            GetOwner()->GetWorld()->GetEntitySystem(ParticleEmitterSystem)
                ->spawnParticles(this, count);
        }

        void ParticleEmitter::SetRate(uint rate)
        {
            _rate = rate;
        }

        uint ParticleEmitter::GetRate(void) const
        {
            return _rate;
        }

        void ParticleEmitter::SetInterval(TimeSpan interval)
        {
            _interval = interval;
        }

        TimeSpan ParticleEmitter::GetInterval(void) const
        {
            return _interval;
        }

        void ParticleEmitter::copy(const ParticleEmitter &other)
        {
            _entity_name = other._entity_name;
            _rate = other._rate;
            _interval = other._interval;
            _type = other._type;
            _timer = other._timer;
            _last_spawned = other._last_spawned;

            for (auto &effect : other._effects)
            {
                _effects.push_back(ParticleEffectRegistrar::Copy(effect));
            }

            _emit = other._emit;
        }

        void ParticleEmitter::addEffect(ParticleEffect *effect)
        {
            UAssert(!utils::IsFlagSet(_mask, effect->GetTypeMask()),
                "Effect already exists.");

            effect->_emitter = this;

            _effects.push_back(effect);

            utils::FlagSet(_mask, effect->GetTypeMask());

            effect->OnInitialize();
        }
    }

    template<>
    Json JsonSerializer::Serialize(ecs::IntervalType &instance)
    {
        return Json(static_cast<int>(instance));
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, ecs::IntervalType &out)
    {
        UAssert(data.is_number(), "Interval type must be serialized as an integer value.");
        out = ecs::IntervalType(data.int_value());
    }

    template<>
    Json JsonSerializer::Serialize(ParticleEffectVector &instance)
    {
        Json::array effect_array;

        for (auto &effect : instance)
        {
            effect_array.push_back(ParticleEffectSerializer::Serialize(effect));
        }

        return Json(effect_array);
    }

    template<>
    void JsonSerializer::Deserialize(const Json &data, ParticleEffectVector &out)
    {
        UAssert(data.is_array(), "effects must be an array.");

        for (auto &item : data.array_items())
        {
            out.push_back(ParticleEffectSerializer::Deserialize(item));
        }
    }
}
