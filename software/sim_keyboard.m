function sim_keyboard(key)
    import java.awt.Robot;
    import java.awt.event.KeyEvent;

    r = Robot();

    switch lower(key)
        case 'up'
            r.keyPress(KeyEvent.VK_UP);
            r.keyRelease(KeyEvent.VK_UP);
        case 'down'
            r.keyPress(KeyEvent.VK_DOWN);
            r.keyRelease(KeyEvent.VK_DOWN);
        otherwise
            keyUpper = upper(key);
            if length(keyUpper) == 1 && keyUpper >= 'A' && keyUpper <= 'Z'
                vkFieldName = ['VK_' keyUpper];
                keyEventClass = java.lang.Class.forName('java.awt.event.KeyEvent');
                field = keyEventClass.getField(vkFieldName);
                vkCode = field.getInt([]);
                r.keyPress(vkCode);
                r.keyRelease(vkCode);
            else
                error('Unsupported key input: %s', key);
            end
    end
end
