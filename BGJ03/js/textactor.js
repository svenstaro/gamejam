TextActor = gamvas.Actor.extend(
{
    create: function(name, text, x, y)
	{
		this._super(name, x, y);
		this.text = text;
		this.color = '#fff';
		this.font = 'bold 16px sans-serif';
	},
	
	draw: function(t)
	{
		var st = gamvas.state.getCurrentState();
        st.c.fillStyle = this.color;
        st.c.font = this.font;
		st.c.fillText(this.text, this.position.x, this.position.y);
	}
});

LevelName = TextActor.extend(
{
    create: function(text)
	{
		this._super("levelname", text, 120, 594);
        this.layer = -100;
	},
    
    draw: function(t)
    {
        var st = gamvas.state.getCurrentState();
        
        st.c.fillStyle = '#000';
        st.c.fillRect(0,575,800,25); 
        
        st.c.fillStyle = this.color;
        st.c.font = this.font;
		st.c.fillText(this.text, this.position.x, this.position.y);
    }
});