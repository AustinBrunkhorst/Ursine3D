
namespace ursine
{
    namespace ecs
    {
        template<class EffectType, typename... Args>
        EffectType *ParticleEmitter::AddEffect(Args&&... args)
        {
            UAssert(IsParticleEffectRegistered(EffectType), "Particle effect is not registered.");

            auto effect = new EffectType(std::forward<Args>(args)...);

            addEffect(effect);

            return effect;
        }

        template<class EffectType>
        EffectType *ParticleEmitter::GetEffect(void)
        {
            if (!HasEffect<EffectType>())
                return nullptr;

            auto id = GetParticleEffectID(EffectType);

            for (auto &effect : _effects)
            {
                if (effect->GetTypeID() == id)
                    return static_cast<EffectType*>(effect);
            }

            return nullptr;
        }

        template<class EffectType>
        void ParticleEmitter::RemoveEffect(void)
        {
            if (!HasEffect<EffectType>())
                return nullptr;

            auto id = GetParticleEffectID(EffectType);

            for (uint i = 0, n = _effects.size(); i < n; ++i)
            {
                auto &effect = _effects[i];

                if (effect->GetTypeID() == id)
                {
                    effect->OnRemove();

                    delete effect;

                    _effects.erase(_effects.front() + i);

                    utils::FlagUnset(_mask, GetParticleEffectMask(EffectType));

                    return;
                }
            }
        }

        template<class EffectType>
        bool ParticleEmitter::HasEffect(void)
        {
            return utils::IsFlagSet(_mask, GetParticleEffectMask(EffectType));
        }
    }
}
